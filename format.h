#ifndef FORMAT_H
#define FORMAT_H

#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#define MAX(x, y) (((x) > (y)) ? (x) : (y))
#define MIN(x, y) (((x) < (y)) ? (x) : (y))

bool validate_input(const char *);
bool is_number(const char *);
int is_tail(const char *);

#endif
