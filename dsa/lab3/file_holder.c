#include "file_holder.h"

#include <stdbool.h>

FILE *currentFile(FILE *in, bool set) {
    static FILE *fp = NULL;
    if (set) fp = in;
    return fp;
}

FILE *setFile(const char *filename) {
    freeFile();
    FILE *fp = fopen(filename, "rb+");
    if (fp == NULL) perror("Could not open the file");
    return currentFile(fp, true);
}

FILE *getFile() { return currentFile(NULL, false); }

void freeFile() {
    FILE *fp = getFile();
    if (fp != NULL) fclose(fp);
    currentFile(NULL, true);
}
