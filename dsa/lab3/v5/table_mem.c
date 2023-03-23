#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include "table.h"

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
            free(table->ks[i].info->info);
            free(table->ks[i].info);
        }
    }
    free(table->ks);
    free(table);
}

// bool insertItem(Table *table, KeyType key, InfoType *info) {
//     for (IndexType i = 0; i < table->msize; i++) {
//         if (table->ks[i].key == 0) {
//             table->ks[i].key = key;
//             table->ks[i].info = malloc(sizeof(Item));
//             if (table->ks[i].info == NULL) {
//                 fprintf(stderr, "Error: Out of memory\n");
//                 exit(EXIT_FAILURE);
//             }
//             table->ks[i].info->key = key;
//             table->ks[i].info->info = info;
//             return true;
//         }
//     }
//     fprintf(stderr, "Error: Table is full\n");
//     return false;
// }

bool insertItem(Table *table, KeyType key, KeyType parKey, InfoType info) {
    // Validate the data
    if (key == 0) {
        TABLE_ERROR("Error: Trying to insert an invalid empty key\n");
        return false;
    }
    // if (parKey != 0 && searchByKey(table, parKey) == NULL) {
    //     TABLE_ERROR("Error: Trying to insert with an invalid parent key\n");
    //     return false;
    // }

    // Check if table is full
    if (table->ks[table->msize - 1].key != 0) {
        TABLE_ERROR("Error: Table is full\n");
        return false;
    }

    // Find the index where the new item should be inserted
    IndexType index = 0;
    while (index < table->msize && table->ks[index].par <= parKey) {
        if (table->ks[index].key == 0) break;
        index++;
    }

    // Shift all items with higher parent key to the right
    for (IndexType i = table->msize - 1; i > index; i--) {
        table->ks[i] = table->ks[i - 1];
    }

    // Insert the new item
    table->ks[index].key = key;
    table->ks[index].par = parKey;
    table->ks[index].info = malloc(sizeof(Item));
    if (table->ks[index].info == NULL) {
        fprintf(stderr, "Error: Out of memory\n");
        exit(EXIT_FAILURE);
    }
    table->ks[index].info->key = key;
    table->ks[index].info->info = malloc(sizeof(InfoType));
    if (table->ks[index].info->info == NULL) {
        fprintf(stderr, "Error: Out of memory\n");
        exit(EXIT_FAILURE);
    }
    *(table->ks[index].info->info) = info;
    return true;
}

bool deleteItem(Table *table, KeyType key) {
    // Validate the data
    if (key == 0) {
        TABLE_ERROR("Error: Trying to delete an invalid empty key\n");
        return false;
    }

    bool found = false;
    for (IndexType i = 0; i < table->msize; i++) {
        if (found) {
            // Shift all following items
            table->ks[i - 1] = table->ks[i];
        } else if (table->ks[i].key == key) {
            // Delete the target item
            free(table->ks[i].info->info);
            free(table->ks[i].info);
            found = true;
        }
    }

    if (found) {
        // Clear the previous last value
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
    for (IndexType i = 0; i < table->msize; i++) {
        if (table->ks[i].key != 0) {
            printf("%u: %u %u\n", table->ks[i].key, table->ks[i].par,
                   *(table->ks[i].info->info));
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
            free(table->ks[i].info->info);
            free(table->ks[i].info);
            table->ks[i].key = 0;
            table->ks[i].par = 0;
            table->ks[i].info = NULL;
        }
    }

    // Insert the the table
    KeyType key;
    KeyType par;
    InfoType info;
    while (fscanf(file, "%u: %u %u", &key, &par, &info) == 3) {
        if (!insertItem(table, key, par, info)) {
            fclose(file);
            return false;
        }
    }
    fclose(file);
    return true;
}

// bool removeByParentKey(Table *table, KeyType parKey) {
//     bool found = false;
//     IndexType it = 0;
//     for (IndexType i = 0; i < table->msize; i++) {
//         if (table->ks[i].par == parKey) {
//             if (found) {
//                 TABLE_ERROR("Error: Duplicate parent keys\n");
//                 return false;
//             }
//             found = true;
//             it = i;
//         }
//     }
//     if (found) {
//         deleteItem(table, table->ks[it].key);
//     } else {
//         TABLE_ERROR("Error: No items found with parent key\n");
//     }
//     return found;
// }

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
    for (IndexType i = 0; i < table->msize; i++) {
        if (table->ks[i].key != 0 && table->ks[i].par == parKey) {
            if (!insertItem(newTable, table->ks[i].key, table->ks[i].par,
                            *(table->ks[i].info->info))) {
                destroyTable(newTable);
                return NULL;
            }
        }
    }
    return newTable;
}
