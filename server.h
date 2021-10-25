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
#include "io.h"

#define MAXPENDING 10   /* Maximum outstanding connection requests */
#define RCVBUFSIZE 32   /* Size of receive buffer */
#define MAXTHREAD 50    /* Maximum thread count */

extern pthread_mutex_t lock;

void run(int, char**);
void die_with_error(char*);  /* Error handling function */
void *handle_tcp_client(void*);   /* TCP client handling function */

#endif
