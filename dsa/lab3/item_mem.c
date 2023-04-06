#include <stdio.h>

#include "item.h"

struct Item {
    InfoType *info;
    KeyType key;
};

bool getInfo(Item *item, InfoType *value) {
    *value = *(item->info);
    return true;
}

bool setInfo(Item *item, InfoType value) {
    *(item->info) = value;
    return true;
}

void allocInfo(Item *item) {
    item->info = calloc(1, sizeof(InfoType));
    if (item->info == NULL) {
        fprintf(stderr, "Error: Out of memory\n");
        exit(EXIT_FAILURE);
    }
}

void freeInfo(Item *item) { free(item->info); }

bool createItem(Table *table, IndexType index, InfoType info) {
    table->ks[index].info = calloc(1, sizeof(Item));
    if (table->ks[index].info == NULL) {
        fprintf(stderr, "Error: Out of memory\n");
        exit(EXIT_FAILURE);
    }
    table->ks[index].info->key = table->ks[index].key;
    allocInfo(table->ks[index].info);
    if (!setInfo(table->ks[index].info, info)) return false;
    return true;
}

KeyType itemKey(Item *item) { return item->key; }
