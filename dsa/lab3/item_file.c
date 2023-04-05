#include "item.h"

#include <stdio.h>

#include "file_holder.h"

struct Item {
    IndexType offset;
    IndexType length;
    KeyType key;
};

bool getInfo(Item *item, InfoType *value) {
    FILE* fp = getFile();
    if (fp == NULL) return false;
    if (fseek(fp, item->offset, SEEK_SET) != 0) return false;
    int value;
    if (fread(&value, 1, item->length, fp) != item->length) return false;
    return true;
}

bool setInfo(Item *item, InfoType value) {
    FILE* fp = getFile();
    if (fp == NULL) return false;
    if (fseek(fp, item->offset, SEEK_SET) != 0) return false;
    if (fwrite(&value, 1, item->length, fp) != item->length) return false;
    return true;
}

void allocInfo(Item *item) {
    return;
}

void freeInfo(Item *item) {}
