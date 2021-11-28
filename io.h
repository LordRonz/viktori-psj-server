#ifndef IO_H
#define IO_H

#include <stdio.h>
#include <stdlib.h>

#define TAIL_BUF 269

int append_to_txt_file(const char *, const int);
int append_to_bin_file(const char *, const int);
void tail(FILE*, int);

#endif
