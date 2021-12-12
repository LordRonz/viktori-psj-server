#include "server.h"

pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;

pthread_t tid[MAXTHREAD] = {0};

volatile bool exit_requested = false;
volatile uint8_t fork_cnt = 0;

void run(const int argc, char **argv) {
    int client_socket_fd;              /* Socket descriptor for client */
    // struct sockaddr_in echoServAddr; /* Local address */
    struct sockaddr_in client_address; /* Client address */
    unsigned int client_len;            /* Length of client address data structure */
    struct addrinfo hints, *res;
    char ip4[INET_ADDRSTRLEN];	     // Space to hold the IPv4 string
    unsigned short max_fork = 1;

    {
        char hostname[100];
        if(gethostname(hostname, sizeof hostname) != 0) {
            perror("gethostname() failed");
        }
        printf("I am %s\n", hostname);
    }

    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_INET;  // use IPv4 or IPv6, whichever
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_PASSIVE;     // fill in my IP for me

    {
        char *server_port = argv[1];  /* First arg:  local port */
        int status = getaddrinfo(argc == 3 ? argv[2] : NULL, server_port, &hints, &res);
        if (status != 0) {
            fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(status));
            exit(EXIT_FAILURE);
        }
    }

    // make a socket:

    /* Socket descriptor for server */
    const int socket_fd = socket(res->ai_family, res->ai_socktype, res->ai_protocol);

    if (socket_fd == -1) {
        die_with_error("socket() failed");
    }

    {
        int yes = 1;
        // lose the pesky "Address already in use" error message
        if (setsockopt(socket_fd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof yes) == -1) {
            die_with_error("setsockopt() failed");
        }
    }

    /* Bind to the local address */
    if (bind(socket_fd, res->ai_addr, res->ai_addrlen) == -1) {
        die_with_error("bind() failed");
    }

    /* Mark the socket so it will listen for incoming connections */
    if (listen(socket_fd, MAXPENDING) == -1) {
        die_with_error("listen() failed");
    }

    {
        struct sockaddr_in *ipv4 = (struct sockaddr_in *) res->ai_addr;
        void *addr = &(ipv4->sin_addr);

        inet_ntop(AF_INET, addr, ip4, INET_ADDRSTRLEN);

        freeaddrinfo(res);

        printf("The IPv4 address is: %s\n", ip4);
    }

    uint_fast8_t thread_cnt = 0;
    struct sigaction sact;
    time_t t;
    pid_t pid = getpid();

    for (;!exit_requested;) {
        // Setup exit handler signal
        sigemptyset(&sact.sa_mask);
        sact.sa_flags = 0;
        sact.sa_handler = catcher;
        sigaction(SIGALRM, &sact, NULL);
        alarm(5);
        time(&t);

	    /* Set the size of the in-out parameter */
        client_len = sizeof client_address;

        /* Wait for a client to connect */
        client_socket_fd = accept(socket_fd, (struct sockaddr *) &client_address, &client_len);

        if (client_socket_fd < 0) {
            if (errno == EINTR || errno == EINVAL) {
                continue;
            }
            perror("accept() failed");
            continue;
        }

        if (thread_cnt >= MAXTHREAD) {
            if (fork_cnt < max_fork) {
                ++fork_cnt;
                printf("Spawning child process %d... from pid %d\n", fork_cnt, pid);
                max_fork += max_fork < MAXFORK ? 1 : 0;
                pid = fork();
                if (pid > 0) {
                    close(client_socket_fd);
                    max_fork = 1;
                    client_socket_fd = 0;
                }
            }
            if (pid == 0) {
                memset(tid, 0, MAXTHREAD);
                thread_cnt = 0;
                pid = getpid();
                continue;
            }

            if (thread_cnt >= MAXTHREAD && fork_cnt >= max_fork) {
                join_thread(thread_cnt);
                thread_cnt = 0;
            }

            if (client_socket_fd == 0) {
                continue;
            }
        }

        /* client socket is connected to a client! */

        printf("Handling client %s port %d \n", inet_ntoa(client_address.sin_addr), client_address.sin_port);

        tcp_thread_args *args = malloc(sizeof *args);

        args->addr = inet_ntoa(client_address.sin_addr);
        args->port = client_address.sin_port;
        args->client_socket_fd = client_socket_fd;

        if (pthread_create(&tid[thread_cnt++], NULL, handle_tcp_client, args) != 0) {
            die_with_error("pthread_create()");
        }
    }
    cancel_thread(thread_cnt);
    join_thread(thread_cnt);
    shutdown(socket_fd, SHUT_RD);
    close(socket_fd);
    exit(EXIT_SUCCESS);
}

void catcher(int sig) {

}

void die_with_error(const char *error_message) {
    perror(error_message);
    exit(EXIT_FAILURE);
}

void join_thread(const int cnt) {
    for (int thread_cnt = 0; thread_cnt < cnt; ++thread_cnt) {
        if (pthread_join(tid[thread_cnt], NULL) != 0) {
            perror("pthread_join()");
        }
    }
}

void cancel_thread(const int cnt) {
    for (int thread_cnt = 0; thread_cnt < cnt; ++thread_cnt) {
        if (pthread_kill(tid[thread_cnt], 0) == 0) {
            if (pthread_cancel(tid[thread_cnt]) != 0) {
                perror("pthread_cancel()");
            }
        }
    }
}

void *handle_tcp_client(void *args) {
    int client_socket = ((tcp_thread_args *)args)->client_socket_fd;
    int port = ((tcp_thread_args *)args)->port;
    char *addr = ((tcp_thread_args *)args)->addr;
    char echo_buffer[RCVBUFSIZE];        /* Buffer for echo string */
    int recv_msg_size;                   /* Size of received message */
    bool valid = false;
    int mode = NORMAL_MODE;
    int n_tail = 0;
    FILE *in = NULL;

    free(args);
    args = NULL;

    /* Receive message from client */
    memset(echo_buffer, 0, RCVBUFSIZE);

    recv_msg_size = recv(client_socket, echo_buffer, RCVBUFSIZE, 0);

    if (recv_msg_size < 0) {
        close(client_socket);
        perror("recv() failed");
        pthread_exit(NULL);
        return NULL;
    }

    echo_buffer[recv_msg_size] = '\0';

    /* Send received string and receive again until end of transmission */
    while (recv_msg_size > 0) {    /* zero indicates end of transmission */
        mode = NORMAL_MODE;
        in = NULL;
        if ((n_tail = is_tail(echo_buffer)) != -1) {
            mode = TAIL_MODE;
            valid = true;
        } else {
            // Validate message
            if (!(valid = validate_input(echo_buffer))) {
                if (send(client_socket, "INVALID INPUT\n", 15, 0) == -1) {
                    perror("send() failed");
                }
            }
        }

        /* Print receiving data to Monitor */
        printf("Received Data: %s\n", echo_buffer);

        if (valid) {
            pthread_mutex_lock(&lock);

            if (mode == NORMAL_MODE && append_to_txt_file(echo_buffer, recv_msg_size) != 0) {
                fputs("Error appending buffer to file\n", stderr);
            } else if (mode == TAIL_MODE) {
                char tail_str[TAIL_BUF];
                char temp_str[TAIL_BUF];
                in = fopen("db.txt", "a+");
                tail(in, n_tail);
                strcpy(tail_str, "");
                while (fgets(temp_str, sizeof temp_str, in)) {
                    strcat(tail_str, temp_str);
                }

                if (send(client_socket, tail_str, strlen(tail_str), 0) == -1) {
                    perror("send() failed");
                }
            }

            pthread_mutex_unlock(&lock);
            /* Echo message back to client */
            if (mode == NORMAL_MODE && send(client_socket, "OK\n", 3, 0) == -1) {
                perror("send() failed");
            }
        }

        memset(echo_buffer, 0, RCVBUFSIZE);

        /* See if there is more data to receive */
        recv_msg_size = recv(client_socket, echo_buffer, RCVBUFSIZE, 0);
        if (recv_msg_size < 0) {
            perror("recv() failed");
        }
    }

    printf("Connection closed with address %s port %d\n", addr, port);
    if (in) {
        fclose(in);
    }
    close(client_socket);    /* Close client socket */
    pthread_exit(NULL);
    return NULL;
}
