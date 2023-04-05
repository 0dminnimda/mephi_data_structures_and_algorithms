#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include "table.h"

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

void freeInfo(Item *item) {
    free(item->info);
}

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

Table *createTable(IndexType msize) {
    Table *table = calloc(1, sizeof(Table));
    if (table == NULL) {
        fprintf(stderr, "Error: Out of memory\n");
        exit(EXIT_FAILURE);
    }
    table->ks = calloc(msize, sizeof(KeySpace));
    if (table->ks == NULL) {
        fprintf(stderr, "Error: Out of memory\n");
        exit(EXIT_FAILURE);
    }
    table->msize = msize;
    return table;
}

void destroyTable(Table *table) {
    for (IndexType i = 0; i < table->msize; i++) {
        if (table->ks[i].key != 0) {
            freeInfo(table->ks[i].info);
            free(table->ks[i].info);
        }
    }
    free(table->ks);
    free(table);
}

IndexType findFirstPlaceByParent(Table *table, KeyType parKey) {
    IndexType left = 0;
    IndexType right = table->msize - 1;

    while (left < right) {
        IndexType mid = (left + right) / 2;

        if (table->ks[mid].par >= parKey || table->ks[mid].key == 0) {
            if (mid == 0) return mid;
            if (table->ks[mid - 1].par < parKey && table->ks[mid - 1].key != 0) {
                return mid;
            }
            right = mid;
        } else {
            left = mid;
        }
    }

    TABLE_ERROR("Error: Unreachable code was reached\n");
    return 0;
}

bool insertItem(Table *table, KeyType key, KeyType parKey, InfoType info) {
    // Validate the data
    if (key == 0) {
        TABLE_ERROR("Error: Trying to insert an invalid empty key\n");
        return false;
    }

    // Check if table is full
    if (table->ks[table->msize - 1].key != 0) {
        TABLE_ERROR("Error: Table is full\n");
        return false;
    }

    // Find the index where the new item should be inserted
    // + check key uniqueness
    // + check parKey existance
    bool parKeyFound = (parKey == 0) || (parKey == table->metaKey);
    IndexType index = -1;
    for (IndexType i = 0; i < table->msize; ++i) {
        if (table->ks[i].key == 0) break;
        if (table->ks[i].key == key) {
            TABLE_ERROR("Error: Trying to insert a duplicate key\n");
            return false;
        }
        if (table->ks[i].par <= parKey) {
            index = i;
        }
        if (table->ks[i].key == parKey) {
            parKeyFound = true;
        }
    }
    if (!parKeyFound) {
        TABLE_ERROR("Error: Parent key is not present in the table\n");
        return false;
    }
    index++;

    // Shift one following item per parent
    KeySpace previous_space = table->ks[index];
    IndexType previous_index = index;
    KeyType previous_parent = table->ks[index].par;
    for (IndexType i = index + 1; i < table->msize; ++i) {
        if (table->ks[i].par != previous_parent) {
            KeySpace tmp = table->ks[i];
            table->ks[i] = previous_space;
            previous_space = tmp;

            previous_index = i;
            previous_parent = table->ks[i].par;
        }
    }

    // Insert the new item
    table->ks[index].key = key;
    table->ks[index].par = parKey;
    return createItem(table, index, info);
}

bool deleteItem(Table *table, KeyType key) {
    // Validate the data
    if (key == 0) {
        TABLE_ERROR("Error: Trying to delete an invalid empty key\n");
        return false;
    }

    IndexType previous_index = -1;
    KeyType previous_parent = -1;
    bool found = false;
    for (IndexType i = 0; i < table->msize; i++) {
        if (found) {
            // Shift one following item per parent
            if (table->ks[i].par != previous_parent) {
                table->ks[previous_index] = table->ks[i - 1];
                previous_index = i - 1;
                previous_parent = table->ks[i].par;
            }
        } else if (table->ks[i].key == key) {
            // Delete the target item
            freeInfo(table->ks[i].info);
            free(table->ks[i].info);
            found = true;
            previous_index = i;
            previous_parent = table->ks[i].par;
        }
    }

    if (found) {
        // End is also a change in the parent
        table->ks[previous_index] = table->ks[table->msize - 1];
        // Clear the last value as it is always empty after delete
        table->ks[table->msize - 1].key = 0;
        table->ks[table->msize - 1].par = 0;
        table->ks[table->msize - 1].info = NULL;
    } else {
        TABLE_ERROR("Error: Item not found\n");
    }
    return found;
}

Item *searchByKey(Table *table, KeyType key) {
    for (IndexType i = 0; i < table->msize; i++) {
        if (table->ks[i].key == key) {
            return table->ks[i].info;
        }
    }
    TABLE_ERROR("Error: Item not found\n");
    return NULL;
}

void outputTable(Table *table) {
    printf("metaKey = %u\n", table->metaKey);
    for (IndexType i = 0; i < table->msize; i++) {
        if (table->ks[i].key != 0) {
            InfoType info = -1;
            if (getInfo(table->ks[i].info, &info))
                printf("[%lu] = %u <- %u: %u\n", i, table->ks[i].key, table->ks[i].par, info);
            else
                printf("[%lu] = error getting the info", i);
        }
    }
}

bool importTable(Table *table, const char *filename) {
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        TABLE_ERROR("Error: File not found\n");
        return false;
    }

    // Clean the table
    for (IndexType i = 0; i < table->msize; i++) {
        if (table->ks[i].key != 0) {
            freeInfo(table->ks[i].info);
            free(table->ks[i].info);
            table->ks[i].key = 0;
            table->ks[i].par = 0;
            table->ks[i].info = NULL;
        }
    }

    // Insert the the table
    fscanf(file, "metaKey = %u", &(table->metaKey));  // it's fine to skip it
    KeyType key;
    KeyType par;
    InfoType info;
    while (fscanf(file, "%u <- %u: %u", &key, &par, &info) == 3) {
        if (!insertItem(table, key, par, info)) {
            fclose(file);
            return false;
        }
    }
    fclose(file);
    return true;
}

bool removeByKeyIfNotParent(Table *table, KeyType key) {
    // Check if the item has a parent key in the table
    bool hasParent = false;
    for (IndexType i = 0; i < table->msize; i++) {
        if (table->ks[i].key != 0 && table->ks[i].par == key) {
            hasParent = true;
            break;
        }
    }

    // If the item has no parent key, remove it from the table
    if (hasParent) {
        TABLE_ERROR("Error: Item has parent key in table\n");
        return false;
    }
    return deleteItem(table, key);
}

Table *searchByParentKey(Table *table, KeyType parKey) {
    Table *newTable = createTable(table->msize);
    newTable->metaKey = parKey;
    for (IndexType i = findFirstPlaceByParent(table, parKey); i < table->msize; i++) {
        KeySpace ks = table->ks[i];
        if (ks.par != parKey || ks.key == 0) break;
        InfoType info = -1;
        if (!getInfo(table->ks[i].info, &info) || !insertItem(newTable, ks.key, ks.par, info)) {
            destroyTable(newTable);
            return NULL;
        }
    }
    return newTable;
}
