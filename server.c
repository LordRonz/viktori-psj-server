#include "server.h"

pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;

pthread_t tid[MAXTHREAD] = {0};

bool exit_requested = false;

void run(int argc, char **argv) {
    int socket_fd;                    /* Socket descriptor for server */
    int client_socket_fd;              /* Socket descriptor for client */
    // struct sockaddr_in echoServAddr; /* Local address */
    struct sockaddr_in client_address; /* Client address */
    unsigned int client_len;            /* Length of client address data structure */
    struct addrinfo hints, *res;
    char ip4[INET_ADDRSTRLEN];	     // Space to hold the IPv4 string

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

    socket_fd = socket(res->ai_family, res->ai_socktype, res->ai_protocol);

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

    sigemptyset(&sact.sa_mask);
    sact.sa_flags = 0;
    sact.sa_handler = catcher;
    sigaction(SIGALRM, &sact, NULL);
    alarm(5);
    time(&t);

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
            if (errno == EINTR) {
                continue;
            }
            perror("accept() failed");
            continue;
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

        if (thread_cnt >= MAXTHREAD) {
            join_thread(thread_cnt);
            thread_cnt = 0;
        }
    }
    join_thread(thread_cnt);
    shutdown(socket_fd, SHUT_RD);
    close(socket_fd);
    exit(EXIT_SUCCESS);
}

void catcher(int sig) {

}

void die_with_error(char *error_message) {
    perror(error_message);
    exit(EXIT_FAILURE);
}

void join_thread(int cnt) {
    for (int thread_cnt = 0; thread_cnt < cnt; ++thread_cnt) {
        if (pthread_join(tid[thread_cnt], NULL) != 0) {
            perror("pthread_join()");
        }
    }
}

void *handle_tcp_client(void *args) {
    int client_socket = ((tcp_thread_args *)args)->client_socket_fd;
    int port = ((tcp_thread_args *)args)->port;
    char *addr = ((tcp_thread_args *)args)->addr;
    char echo_buffer[RCVBUFSIZE];        /* Buffer for echo string */
    int recv_msg_size;                   /* Size of received message */

    free(args);

    /* Receive message from client */
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
        /* Print receiving data to Monitor */
        printf("Received Data: %s\n", echo_buffer);

        pthread_mutex_lock(&lock);

        if (append_to_txt_file(echo_buffer, recv_msg_size) != 0) {
            fputs("Error appending buffer to file\n", stderr);
        }

        pthread_mutex_unlock(&lock);

        memset(echo_buffer, 0, RCVBUFSIZE);

        /* Echo message back to client */
        if (send(client_socket, "OK\n", 4, 0) == -1) {
            perror("send() failed");
        }

        /* See if there is more data to receive */
        recv_msg_size = recv(client_socket, echo_buffer, RCVBUFSIZE, 0);
        if (recv_msg_size < 0) {
            perror("recv() failed");
        }
    }

    printf("Connection closed by address %s port %d\n", addr, port);
    close(client_socket);    /* Close client socket */
    pthread_exit(NULL);
    return NULL;
}
