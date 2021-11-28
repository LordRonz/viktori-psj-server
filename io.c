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

void tail(FILE* in, int n)
{
    int count = 0;  // To count '\n' characters

    // unsigned long long pos (stores upto 2^64 – 1
    // chars) assuming that long long int takes 8
    // bytes
    unsigned long long pos;

    // Go to End of file
    if (fseek(in, 0, SEEK_END)) {
        perror("fseek() failed");
    }
    else {
        // pos will contain no. of chars in
        // input file.
        pos = ftell(in);

        // search for '\n' characters
        while (pos) {
            // Move 'pos' away from end of file.
            if (!fseek(in, --pos, SEEK_SET)) {
                if (fgetc(in) == '\n')

                    // stop reading when n newlines
                    // is found
                    if (count++ == n)
                        break;
            }
            else {
                perror("fseek() failed");
            }
        }

        // // print last n lines
        // printf("Printing last %d lines -\n", n);
        // while (fgets(str, sizeof(str), in)) {
        //     printf("%s", str);
        // }
    }
    printf("\n\n");
}
