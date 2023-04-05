#include "file_holder.h"

FILE *currentFile(FILE *in) {
    static FILE *fp = NULL;
    if (in != NULL) fp = in;
    return fp;
}

FILE *setFile(char *filename) {
    FILE* fp = fopen(filename, "rb+");
    if (fp == NULL) {
        perror("Could not open the file");
        return NULL;
    }
    return currentFile(fp);
}

FILE *getFile() {
    return currentFile(NULL);
}
