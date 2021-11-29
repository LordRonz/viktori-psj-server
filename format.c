#include "format.h"

bool validate_input(const char * input) {
    char *cpy_input = strdup(input);
    char *token;

    token = strtok(cpy_input, " ");
    if (token == NULL || token[0] < '1' || token[0] > '9') {
        free(cpy_input);
        return false;
    }
    if (!is_number(token)) {
        free(cpy_input);
        return false;
    }

    token = strtok(NULL, " ");
    if (token == NULL || token[0] < '1' || token[0] > '9') {
        free(cpy_input);
        return false;
    }
    if (!is_number(token)) {
        free(cpy_input);
        return false;
    }

    token = strtok(NULL, " ");
    if (token == NULL || token[0] < '1' || token[0] > '9') {
        free(cpy_input);
        return false;
    }
    if (!is_number(token)) {
        free(cpy_input);
        return false;
    }

    token = strtok(NULL, " ");
    free(cpy_input);
    return token == NULL;
}

bool is_number(const char * str) {
    for (unsigned int i = 0, j = strlen(str); i < j; ++i) {
        if (str[i] < '0' || str[i] > '9') {
            if (!(str[i] == '\n' && i + 1 == j) && !(str[i] == '\r' && i + 2 == j)) {
                return false;
            }
        }
    }
    return true;
}

int is_tail(const char * input) {
    char *cpy_input = strdup(input);
    char *token;
    char *ptr;

    token = strtok(cpy_input, " ");
    if (token == NULL || strcmp(token, "tail")) {
        free(cpy_input);
        return -1;
    }

    token = strtok(NULL, " ");
    if (token == NULL || token[0] < '1' || token[0] > '9') {
        free(cpy_input);
        return 5;
    }
    if (!is_number(token)) {
        free(cpy_input);
        return 5;
    }

    return MAX(MIN(strtol(token, &ptr, 10), 10), 1);
}
