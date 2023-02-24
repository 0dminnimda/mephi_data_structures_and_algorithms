#include "input.h"

#include <stdio.h>
#include <stdlib.h>

#include "error.h"

char *fread_line(FILE *stream) {
    size_t len = 0;
    size_t cap = 0;
    char *buf = NULL;

    while (1) {
        if (cap == len) {
            cap = cap * 2 + 1;
            char *new_buf = realloc(buf, cap * sizeof(char));
            if (new_buf == NULL) {
                free(buf);
                return NULL;
            }
            buf = new_buf;
        }

        buf[len] = fgetc(stream);
        if (buf[len] == EOF) {
            free(buf);
            return NULL;
        }
        if (buf[len] == '\n') break;
        len++;
    }

    buf[len] = '\0';
    return buf;
}

char *read_line() { return fread_line(stdin); }
