#include "format.h"

bool validate_input(const char * input) {
    char cpy_input[strlen(input) + 1];
    char *token;

    strcpy(cpy_input, input);

    token = strtok(cpy_input, " ");
    if (token == NULL || (!strcmp(token, "0") && !strcmp(token, "1"))) {
        return false;
    }

    token = strtok(NULL, " ");
    if (token == NULL || token[0] < '1' || token[0] > '9') {
        return false;
    }
    if (!is_number(token)) {
        return false;
    }

    token = strtok(NULL, " ");
    if (token == NULL || token[0] < '1' || token[0] > '9') {
        return false;
    }
    if (!is_number(token)) {
        return false;
    }

    token = strtok(NULL, " ");
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
