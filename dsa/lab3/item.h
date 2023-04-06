#ifndef ITEM_H
#define ITEM_H

typedef struct Item Item;

#include <stdbool.h>

#include "table.h"
#include "typedefs.h"

bool getInfo(Item *item, InfoType *value);
bool setInfo(Item *item, InfoType value);
void allocInfo(Item *item);
void freeInfo(Item *item);
bool createItem(Table *table, IndexType index, InfoType info);
KeyType itemKey(Item *item);

#endif  // ITEM_H
