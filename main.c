#include "server.h"

void sigint_handler(int sig) {
    puts("Terminated");
    exit(EXIT_SUCCESS);
}

int main(int argc, char **argv) {
    signal(SIGINT, sigint_handler);
    if (argc != 2 && argc != 3) {    /* Test for correct number of arguments */
        fprintf(stderr, "Usage:  %s <Server Port> <Server Address (OPTIONAL)>\n", argv[0]);
        return EXIT_FAILURE;
    }
    run(argc, argv);
}
