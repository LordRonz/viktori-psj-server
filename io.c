#include "io.h"

int append_to_txt_file(const char *buf, const int size) {
    FILE *fptr = NULL;
    fptr = fopen("db.txt", "a+");

    if (fptr == NULL) {
        perror("fopen()");
        return EXIT_FAILURE;
    }

    if (fputs(buf, fptr) == EOF) {
        perror("fputs()");
        return EXIT_FAILURE;
    }

    if (fclose(fptr) != 0) {
        perror("fclose()");
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}

int append_to_bin_file(const char *buf, const int size) {
    FILE *fptr = NULL;
    fptr = fopen("db.bin", "a+b");

    if (fptr == NULL) {
        perror("fopen()");
        return EXIT_FAILURE;
    }

    if (fwrite(buf, size, 1, fptr) < 1) {
        perror("fwrite()");
        return EXIT_FAILURE;
    }

    if (fclose(fptr) != 0) {
        perror("fclose()");
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
