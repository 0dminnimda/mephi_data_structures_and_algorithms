#ifndef TABLE_H
#define TABLE_H

#ifndef NO_TABLE_PRINT_ERRORS
    #define TABLE_ERROR(msg) fprintf(stderr, msg)
#else
    #define TABLE_ERROR(msg)
#endif

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

typedef unsigned int KeyType;
typedef unsigned int InfoType;
typedef size_t IndexType;

typedef struct {
    InfoType *info;
    KeyType key;
} Item;

typedef struct {
    KeyType key;
    KeyType par;
    Item *info;
} KeySpace;

typedef struct {
    KeySpace *ks;
    IndexType msize;
    KeyType metaKey;
} Table;

Table *createTable(IndexType msize);
void destroyTable(Table *table);
IndexType findFirstPlaceByParent(Table *table, KeyType parKey);
bool insertItem(Table *table, KeyType key, KeyType parKey, InfoType info);
bool deleteItem(Table *table, KeyType key);
Item *searchByKey(Table *table, KeyType key);
void outputTable(Table *table);
bool importTable(Table *table, const char *filename);
bool removeByKeyIfNotParent(Table *table, KeyType key);
Table *searchByParentKey(Table *table, KeyType parKey);

#endif  // TABLE_H
