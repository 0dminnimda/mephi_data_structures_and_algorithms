#include <stdio.h>

#include "file_holder.h"
#include "item.h"

struct Item {
    IndexType offset;
    IndexType length;
    KeyType key;
};

bool getInfo(Item *item, InfoType *value) {
    FILE *fp = getFile();
    if (fp == NULL) return false;
    if (fseek(fp, item->offset, SEEK_SET) != 0) return false;
    int value;
    if (fread(&value, item->length, 1, fp) != item->length) return false;
    return true;
}

bool setInfo(Item *item, InfoType value) {
    FILE *fp = getFile();
    if (fp == NULL) return false;
    if (fseek(fp, item->offset, SEEK_SET) != 0) return false;
    if (fwrite(&value, item->length, 1, fp) != item->length) return false;
    return true;
}

void allocInfo(Item *item) {}

void freeInfo(Item *item) {}

bool createItem(Table *table, IndexType index, InfoType info) {
    // todo
    return true;
}

KeyType itemKey(Item *item) { return item->key; }
