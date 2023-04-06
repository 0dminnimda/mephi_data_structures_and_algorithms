/*
Write program in C++, but leave the actual implementation blank (aka make a skeleton):

- Library for vector-based table
- Console application to work with library table interactively

Library implementations:

1. In-memory
2. External memory (binary file)

Vector-based table:

`table.h`:
```c
typedef unsigned int KeyType;
typedef unsigned int InfoType;
typedef size_t IndexType;

struct Item {
    InfoType *info; // actual data
    KeyType key; // backreference to KeySpace
};

struct KeySpace {
    KeyType key;
    KeyType par; // nullable
    Item *info;
};

struct Table {
    KeySpace *ks;
    IndexType msize; // max key space size
};
```

No dup keys/must have parent key or null.
Ordered by parent key. Parent key may duplicate.

Operations:

- Insert element with unique key
- Delete element by key
- Search by key (returns an iterator of matches)
- Output to standard stream
- Import from text file
- Remove by key if no parent keys in table = key
- Search by parent key value, return new table of matches

Use buffering and caching for external memory

Requirements:

- No globals
- If input incorrect, show error, then continue
- If error in table op, show it, then continue
- Modified struct for external memory (add length/offset)
- Use `fread`/`fwrite` for files (with length)
- Correct memory usage
*/

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>

// #include "common/input.h"
#include "file_holder.h"
#include "table.h"

bool file_exists(const char *filename) {
    struct stat buffer;
    return stat(filename, &buffer) == 0;
}

#define FIELD_SIZE(structure, field) sizeof(((structure *)NULL)->field)
#define TABLE_OFFSET (FIELD_SIZE(Table, msize) + FIELD_SIZE(Table, metaKey))
#define KEYSPACE_OFFSET \
    (FIELD_SIZE(KeySpace, key) + FIELD_SIZE(KeySpace, par) + sizeof(InfoType))
#define KEYSPACE_INDEX_OFFSET(index) (TABLE_OFFSET + KEYSPACE_OFFSET * index)

bool setKeyspaceAsFields(IndexType index, KeyType key, KeyType par, InfoType info) {
    FILE *fp = getFile();
    if (fp == NULL) { return false; }
    if (fseek(fp, KEYSPACE_INDEX_OFFSET(index), SEEK_SET)) { return false; }
    if (!fwrite(&key, sizeof(KeyType), 1, fp)) { return false; }
    if (!fwrite(&par, sizeof(KeyType), 1, fp)) { return false; }
    if (!fwrite(&info, sizeof(InfoType), 1, fp)) { return false; }
    return true;
}

bool setKeyspace(IndexType index, const KeySpace *keyspace) {
    return setKeyspaceAsFields(index, keyspace->key, keyspace->par,
                               *(keyspace->info->info));
}

KeySpace *getKeyspace(IndexType index) {
    FILE *fp = getFile();
    if (fp == NULL) { return false; }

    KeySpace *keyspace = malloc(sizeof(KeySpace));
    Item *item = malloc(sizeof(Item));
    InfoType *info = malloc(sizeof(InfoType));
    if (!keyspace || !item || !info) {
        fprintf(stderr, "Error: Out of memory\n");
        goto fail;
    }

    if (fseek(fp, KEYSPACE_INDEX_OFFSET(index), SEEK_SET)) { goto fail; }
    if (!fread(&keyspace->key, sizeof(KeyType), 1, fp)) { goto fail; }
    if (!fread(&keyspace->par, sizeof(KeyType), 1, fp)) { goto fail; }
    if (!fread(info, sizeof(InfoType), 1, fp)) { goto fail; }

    item->info = info;
    keyspace->info = item;

    return keyspace;

fail:
    free(info);
    free(item);
    free(keyspace);

    return NULL;
}

void freeKeyspace(KeySpace *keyspace) {
    if (!keyspace) return;
    if (keyspace->info) { free(keyspace->info->info); }
    free(keyspace->info);
    free(keyspace);
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
    if (file_exists(filename)) {
        FILE *fp = getFile();
        if (fp == NULL) { return false; }
        if (fseek(fp, 0, SEEK_SET)) { return false; }
        if (fread(&table->metaKey, sizeof(KeyType), 1, fp) != 1) { return false; }
        if (fread(&table->msize, sizeof(IndexType), 1, fp) != 1) { return false; }
    } else {
        FILE *fp = setFile(filename);
        if (fp == NULL) { return false; }
        if (fseek(fp, 0, SEEK_SET)) { return false; }
        if (fwrite(&table->metaKey, sizeof(KeyType), 1, fp) != 1) { return false; }
        if (fwrite(&table->msize, sizeof(IndexType), 1, fp) != 1) { return false; }
    }
    return true;
}

void destroyTable(Table *table) { free(table); }

IndexType findFirstPlaceByParent(Table *table, KeyType parKey) {
    IndexType left = 0;
    IndexType right = table->msize - 1;

    while (left < right) {
        IndexType mid = (left + right) / 2;
        KeySpace *ks = getKeyspace(mid);

        if (ks->par >= parKey || ks->key == 0) {
            freeKeyspace(ks);
            KeySpace *ks = getKeyspace(mid - 1);
            if (mid == 0) return mid;
            if (ks->par < parKey && ks->key != 0) {
                freeKeyspace(ks);
                return mid;
            }
            freeKeyspace(ks);
            right = mid;
        } else {
            left = mid;
        }
        freeKeyspace(ks);

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
    KeySpace *ks = getKeyspace(table->msize - 1);
    if (ks->key != 0) {
        freeKeyspace(ks);
        TABLE_ERROR("Error: Table is full\n");
        return false;
    }
    freeKeyspace(ks);

    // Find the index where the new item should be inserted
    // + check key uniqueness
    // + check parKey existance
    bool parKeyFound = (parKey == 0) || (parKey == table->metaKey);
    IndexType index = -1;
    for (IndexType i = 0; i < table->msize; ++i) {
        freeKeyspace(ks);
        ks = getKeyspace(i);
        if (ks->key == 0) break;
        if (ks->key == key) {
            TABLE_ERROR("Error: Trying to insert a duplicate key\n");
            return false;
        }
        if (ks->par <= parKey) { index = i; }
        if (ks->key == parKey) { parKeyFound = true; }
    }
    freeKeyspace(ks);
    if (!parKeyFound) {
        TABLE_ERROR("Error: Parent key is not present in the table\n");
        return false;
    }
    index++;

    // Shift one following item per parent
    KeySpace *previous_space = getKeyspace(index);
    IndexType previous_index = index;
    KeyType previous_parent = previous_space->par;
    for (IndexType i = index + 1; i < table->msize; ++i) {
        freeKeyspace(ks);
        ks = getKeyspace(i);
        if (ks->par != previous_parent) {
            KeySpace *tmp = ks;
            setKeyspace(i, previous_space);
            previous_space = tmp;

            previous_index = i;
            freeKeyspace(ks);
            ks = getKeyspace(i);
            previous_parent = ks->par;
        }
    }
    freeKeyspace(ks);
    freeKeyspace(previous_space);

    // Insert the new item
    return setKeyspaceAsFields(index, key, parKey, info);
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
        KeySpace *ks = getKeyspace(i);
        if (found) {
            // Shift one following item per parent
            if (ks->par != previous_parent) {
                freeKeyspace(ks);
                ks = getKeyspace(i - 1);
                setKeyspace(previous_index, ks);
                previous_index = i - 1;
                previous_parent = ks->par;
                freeKeyspace(ks);
            }
        } else if (ks->key == key) {
            // Delete the target item
            found = true;
            previous_index = i;
            previous_parent = ks->par;
        }
        freeKeyspace(ks);
    }

    if (found) {
        // End is also a change in the parent
        KeySpace *ks = getKeyspace(table->msize - 1);
        setKeyspace(previous_index, ks);
        freeKeyspace(ks);
        // Clear the last value as it is always empty after delete
        setKeyspaceAsFields(table->msize - 1, 0, 0, 0);
    } else {
        TABLE_ERROR("Error: Item not found\n");
    }
    return found;
}

Item *searchByKey(Table *table, KeyType key) {
    KeySpace *ks = NULL;
    for (IndexType i = 0; i < table->msize; i++) {
        freeKeyspace(ks);
        ks = getKeyspace(i);
        if (ks->key == key) {
            Item *res = ks->info;
            free(ks);
            return res;
        }
    }
    freeKeyspace(ks);
    TABLE_ERROR("Error: Item not found\n");
    return NULL;
}

void outputTable(Table *table) {
    printf("metaKey = %u\n", table->metaKey);
    for (IndexType i = 0; i < table->msize; i++) {
        KeySpace *ks = getKeyspace(i);
        if (ks->key != 0) {
            printf("[%zu] = %u <- %u: %u\n", i, ks->key, ks->par, *(ks->info->info));
        }
        freeKeyspace(ks);
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
        KeySpace *ks = getKeyspace(i);
        if (ks->key != 0) {
            ks->key = 0;
            ks->par = 0;
            ks->info->info = 0;
        }
        setKeyspace(i, ks);
        freeKeyspace(ks);
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
        KeySpace *ks = getKeyspace(i);
        if (ks->key != 0 && ks->par == key) {
            hasParent = true;
            break;
        }
        freeKeyspace(ks);
    }

    // If the item has no parent key, remove it from the table
    if (hasParent) {
        TABLE_ERROR("Error: Item has parent key in table\n");
        return false;
    }
    return deleteItem(table, key);
}

Table *searchByParentKey(Table *table, KeyType parKey) {
    Table *newTable = createTable(table->msize, parKey);
    KeySpace *ks = NULL;
    for (IndexType i = findFirstPlaceByParent(table, parKey); i < table->msize; i++) {
        freeKeyspace(ks);
        ks = getKeyspace(i);
        if (ks->par != parKey || ks->key == 0) break;
        if (!insertItem(newTable, ks->key, ks->par, *(ks->info->info))) {
            freeKeyspace(ks);
            destroyTable(newTable);
            return NULL;
        }
    }
    freeKeyspace(ks);
    return newTable;
}
