#ifndef SERVER_H
#define SERVER_H

#include <netdb.h>
#include <netinet/in.h>
#include <stdio.h>      /* for printf() and fprintf() */
#include <sys/socket.h> /* for socket(), bind(), and connect() */
#include <arpa/inet.h>  /* for sockaddr_in and inet_ntoa() */
#include <stdlib.h>     /* for atoi() and exit() */
#include <string.h>     /* for memset() */
#include <sys/types.h>
#include <unistd.h>     /* for close() */
#include <errno.h>
#include <signal.h>
#include <pthread.h>
#include <stdint.h>
#include <stdbool.h>
#include <time.h>
#include "io.h"
#include "format.h"

#define MAXPENDING 5   /* Maximum outstanding connection requests */
#define RCVBUFSIZE 1000 /* Size of receive buffer */
#define MAXTHREAD 20    /* Maximum thread count */
#define MAXFORK 4       /* Maximum fork count*/
#define NORMAL_MODE 0
#define TAIL_MODE 1

extern pthread_mutex_t lock;
extern pthread_t tid[MAXTHREAD];
extern volatile bool exit_requested;
extern volatile uint8_t fork_cnt;

typedef struct tcp_thread_args {
    char *addr;
    int port;
    int client_socket_fd;
} tcp_thread_args;

void run(const int, char**);
void die_with_error(const char*);  /* Error handling function */
void *handle_tcp_client(void*);   /* TCP client handling function */
void join_thread(const int);
void cancel_thread(const int);
void catcher(int);

#endif
