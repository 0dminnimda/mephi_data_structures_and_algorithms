#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>

#include "file_holder.h"
#include "table.h"

bool file_exists(const char *filename) {
    struct stat buffer;
    return stat(filename, &buffer) == 0;
}

#define FIELD_SIZE(structure, field) sizeof(((structure *)NULL)->field)
#define TABLE_OFFSET (sizeof(IndexType) + sizeof(KeyType))
#define KEYSPACE_OFFSET (sizeof(KeyType) + sizeof(KeyType) + sizeof(InfoType))
#define KEYSPACE_INDEX_OFFSET(index) (TABLE_OFFSET + KEYSPACE_OFFSET * index)

bool setKeyspaceAsFields(Table *table, IndexType index, KeyType key, KeyType par,
                         InfoType info) {
    FILE *fp = table->fp;
    if (fp == NULL) { return false; }
    if (fseek(fp, KEYSPACE_INDEX_OFFSET(index), SEEK_SET)) { return false; }
    if (!fwrite(&key, sizeof(KeyType), 1, fp)) { return false; }
    if (!fwrite(&par, sizeof(KeyType), 1, fp)) { return false; }
    if (!fwrite(&info, sizeof(InfoType), 1, fp)) { return false; }
    return true;
}

bool setKeyspace(Table *table, IndexType index, const KeySpace *ks) {
    if (!ks) return setKeyspaceAsFields(table, index, 0, 0, 0);
    return setKeyspaceAsFields(table, index, ks->key, ks->par, *(ks->info->info));
}

KeySpace *getKeyspace(Table *table, IndexType index) {
    FILE *fp = table->fp;
    if (fp == NULL) { return false; }

    KeySpace *ks = calloc(1, sizeof(KeySpace));
    Item *item = calloc(1, sizeof(Item));
    InfoType *info = calloc(1, sizeof(InfoType));
    if (!ks || !item || !info) {
        fprintf(stderr, "Error: Out of memory\n");
        goto fail;
    }

    if (fseek(fp, KEYSPACE_INDEX_OFFSET(index), SEEK_SET)) { goto fail; }
    if (!fread(&ks->key, sizeof(KeyType), 1, fp)) { goto fail; }
    if (!fread(&ks->par, sizeof(KeyType), 1, fp)) { goto fail; }
    if (!fread(info, sizeof(InfoType), 1, fp)) { goto fail; }

    item->info = info;
    ks->info = item;

    return ks;

fail:
    free(info);
    free(item);
    free(ks);

    return NULL;
}

void freeKeyspace(KeySpace **ks_ptr) {
    KeySpace *ks = *ks_ptr;
    if (!ks) return;
    if (ks->info) { free(ks->info->info); }
    free(ks->info);
    free(ks);
    *ks_ptr = NULL;
}

Table *createTable(IndexType msize, KeyType metaKey) {
    Table *table = calloc(1, sizeof(Table));
    if (table == NULL) {
        fprintf(stderr, "Error: Out of memory\n");
        exit(EXIT_FAILURE);
    }
    table->msize = msize;
    table->metaKey = metaKey;
    return table;
}

bool syncTableWithFile(Table *table, const char *filename) {
    bool existed = file_exists(filename);

    table->fp = fopen(filename, existed ? "rb+" : "wb+");
    if (table->fp == NULL) { return false; }
    if (fseek(table->fp, 0, SEEK_SET)) { return false; }

    if (existed) {
        if (fread(&table->msize, sizeof(IndexType), 1, table->fp) != 1) { return false; }
        if (fread(&table->metaKey, sizeof(KeyType), 1, table->fp) != 1) { return false; }
    } else {
        if (fwrite(&table->msize, sizeof(IndexType), 1, table->fp) != 1) { return false; }
        if (fwrite(&table->metaKey, sizeof(KeyType), 1, table->fp) != 1) { return false; }
        for (IndexType i = 0; i < table->msize; ++i) { setKeyspace(table, i, NULL); }
    }
    return true;
}

void destroyTable(Table *table) {
    if (table && table->fp != NULL) fclose(table->fp);
    free(table);
}

IndexType findFirstPlaceByParent(Table *table, KeyType parKey) {
    IndexType left = 0;
    IndexType right = table->msize - 1;

    while (left < right) {
        IndexType mid = (left + right) / 2;
        KeySpace *ks = getKeyspace(table, mid);

        if (!ks || ks->par >= parKey || ks->key == 0) {
            freeKeyspace(&ks);
            KeySpace *ks = getKeyspace(table, mid - 1);
            if (mid == 0) return mid;
            if (ks && ks->par < parKey && ks->key != 0) {
                freeKeyspace(&ks);
                return mid;
            }
            freeKeyspace(&ks);
            right = mid;
        } else {
            left = mid;
        }
        freeKeyspace(&ks);

        // would cause infinite loop, return best possible option
        if (left == right - 1) { return left; }
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
    KeySpace *ks = getKeyspace(table, table->msize - 1);
    if (ks && ks->key != 0) {
        freeKeyspace(&ks);
        TABLE_ERROR("Error: Table is full\n");
        return false;
    }
    freeKeyspace(&ks);

    // Find the index where the new item should be inserted
    // + check key uniqueness
    // + check parKey existance
    bool parKeyFound = (parKey == 0) || (parKey == table->metaKey);
    IndexType index = -1;
    for (IndexType i = 0; i < table->msize; ++i) {
        freeKeyspace(&ks);
        ks = getKeyspace(table, i);
        if (!ks || ks->key == 0) break;
        if (ks->key == key) {
            freeKeyspace(&ks);
            TABLE_ERROR("Error: Trying to insert a duplicate key\n");
            return false;
        }
        if (ks->par <= parKey) { index = i; }
        if (ks->key == parKey) { parKeyFound = true; }
    }
    freeKeyspace(&ks);
    if (!parKeyFound) {
        freeKeyspace(&ks);
        TABLE_ERROR("Error: Parent key is not present in the table\n");
        return false;
    }
    index++;

    // Shift one following item per parent
    KeySpace *previous_space = getKeyspace(table, index);
    IndexType previous_index = index;
    KeyType previous_parent = previous_space ? previous_space->par : 0;
    for (IndexType i = index + 1; i < table->msize; ++i) {
        ks = getKeyspace(table, i);
        if (!ks || ks->par != previous_parent) {
            setKeyspace(table, i, previous_space);
            freeKeyspace(&previous_space);
            previous_space = ks;

            previous_index = i;
            previous_parent = previous_space ? previous_space->par : 0;
        } else {
            freeKeyspace(&ks);
        }
    }
    freeKeyspace(&previous_space);

    // Insert the new item
    return setKeyspaceAsFields(table, index, key, parKey, info);
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
        KeySpace *ks = getKeyspace(table, i);
        if (found) {
            // Shift one following item per parent
            if (ks && ks->par != previous_parent) {
                freeKeyspace(&ks);
                ks = getKeyspace(table, i - 1);
                setKeyspace(table, previous_index, ks);
                previous_index = i - 1;
                previous_parent = ks->par;
            }
        } else if (ks && ks->key == key) {
            // Delete the target item
            found = true;
            previous_index = i;
            previous_parent = ks->par;
        }
        freeKeyspace(&ks);
    }

    if (found) {
        // End is also a change in the parent
        KeySpace *ks = getKeyspace(table, table->msize - 1);
        if (ks) setKeyspace(table, previous_index, ks);
        freeKeyspace(&ks);
        // Clear the last value as it is always empty after delete
        setKeyspaceAsFields(table, table->msize - 1, 0, 0, 0);
    } else {
        TABLE_ERROR("Error: Item not found\n");
    }
    return found;
}

Item *searchByKey(Table *table, KeyType key) {
    KeySpace *ks = NULL;
    for (IndexType i = 0; i < table->msize; i++) {
        freeKeyspace(&ks);
        ks = getKeyspace(table, i);
        if (ks && ks->key == key) {
            Item *res = ks->info;
            free(ks);
            return res;
        }
    }
    freeKeyspace(&ks);
    TABLE_ERROR("Error: Item not found\n");
    return NULL;
}

void outputTable(Table *table) {
    printf("metaKey = %u\n", table->metaKey);
    for (IndexType i = 0; i < table->msize; i++) {
        KeySpace *ks = getKeyspace(table, i);
        if (ks && ks->key != 0)
            printf("[%zu] = %u <- %u: %u\n", i, ks->key, ks->par, *(ks->info->info));
        else if (!ks)
            printf("[%zu] = error getting the info\n", i);
        freeKeyspace(&ks);
    }
}

bool importTable(Table *table, const char *filename) {
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        TABLE_ERROR("Error: File not found\n");
        return false;
    }

    // Clean the table
    for (IndexType i = 0; i < table->msize; i++) { setKeyspace(table, i, NULL); }

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
        KeySpace *ks = getKeyspace(table, i);
        if (ks && ks->key != 0 && ks->par == key) {
            hasParent = true;
            break;
        }
        freeKeyspace(&ks);
    }

    // If the item has no parent key, remove it from the table
    if (hasParent) {
        TABLE_ERROR("Error: Item has parent key in table\n");
        return false;
    }
    return deleteItem(table, key);
}

KeySpace *nextWithParent(Table *table, KeyType parKey) {
    static IndexType index = 0;
    static KeyType par = 0;

    if (parKey != -1) {
        par = parKey;
        index = findFirstPlaceByParent(table, par);
        return NULL;
    }

    KeySpace *ks = getKeyspace(table, index);
    if (ks->par != par || ks->key == 0) {
        freeKeyspace(&ks);
        return NULL;
    }
    ++index;
    return ks;
}

Table *searchByParentKey(Table *table, KeyType parKey) {
    Table *newTable = createTable(table->msize, parKey);
    newTable->fp = fopen("parent_search_result.dat", "wb+");
    if (newTable->fp == NULL) {
        destroyTable(newTable);
        return NULL;
    }

    KeySpace *ks = nextWithParent(table, parKey);
    while ((ks = nextWithParent(table, -1))) {
        if (!insertItem(newTable, ks->key, ks->par, *(ks->info->info))) {
            destroyTable(newTable);
            newTable = NULL;
            break;
        }
        freeKeyspace(&ks);
    }
    freeKeyspace(&ks);
    return newTable;
}
