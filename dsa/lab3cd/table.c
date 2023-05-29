#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifndef NO_TABLE_PRINT_ERRORS
    #define TABLE_ERROR(msg) fprintf(stderr, msg)
#else
    #define TABLE_ERROR(msg)
#endif

typedef size_t IndexType;
typedef unsigned int KeyType;
typedef unsigned int RelType;
typedef unsigned int Item;

typedef struct Node {
    RelType release;
    Item *info;
    struct Node *next;
} Node;

typedef struct KeySpace {
    KeyType key;
    Node *node;
    struct KeySpace *next;
} KeySpace;

typedef struct Table {
    KeySpace *ks;
    IndexType msize;
} Table;

#define FOR_KEY_SPACE(name, init) \
    for (KeySpace *name = init; name != NULL; name = name->next)

#define FOR_NODE(name, init) for (Node *name = init; name != NULL; name = name->next)

Table *create_table(IndexType msize) {
    Table *table = calloc(1, sizeof(Table));
    table->ks = calloc(msize, sizeof(KeySpace));
    table->msize = msize;
    return table;
}

IndexType hash(Table *table, KeyType key) { return key % table->msize; }

KeySpace *get_ks(Table *table, KeyType key) {
    IndexType index = hash(table, key);
    KeySpace *target_ks = NULL;
    KeySpace *first_ks = table->ks + index;

    FOR_KEY_SPACE(ks, first_ks) {
        if (ks->key == key) {
            target_ks = ks;
            break;
        }
    }

    if (!target_ks) {
        if (first_ks->node) {
            target_ks = calloc(1, sizeof(KeySpace));
            target_ks->next = first_ks->next;
            first_ks->next = target_ks;
        } else {
            target_ks = first_ks;
        }
        target_ks->key = key;
    }

    return target_ks;
}

bool insert(Table *table, KeyType key, Item info) {
    KeySpace *target_ks = get_ks(table, key);

    Node *new_node = calloc(1, sizeof(Node));
    new_node->info = calloc(1, sizeof(Item));
    *(new_node->info) = info;
    new_node->next = target_ks->node;
    if (target_ks->node) {
        new_node->release = target_ks->node->release + 1;
    } else {
        new_node->release = 1;
    }
    target_ks->node = new_node;
    return true;
}

bool insert_with_release(Table *table, KeyType key, Item info, RelType release) {
    KeySpace *target_ks = get_ks(table, key);

    Node *new_node = calloc(1, sizeof(Node));
    new_node->info = calloc(1, sizeof(Item));
    *(new_node->info) = info;
    new_node->release = release;

    Node *prev_node = NULL;
    Node *curr_node = target_ks->node;
    while (curr_node != NULL && curr_node->release > release) {
        prev_node = curr_node;
        curr_node = curr_node->next;
    }

    if (prev_node == NULL) {
        target_ks->node = new_node;
    } else {
        new_node->next = prev_node->next;
        prev_node->next = new_node;
    }

    return true;
}

KeySpace *search(Table *table, KeyType key) {
    IndexType index = hash(table, key);
    FOR_KEY_SPACE(ks, (table->ks + index)) {
        if (ks->key == key) { return ks; }
    }
    return NULL;
}

Node *search_spesific(Table *table, KeyType key, RelType release) {
    KeySpace *ks = search(table, key);
    if (ks == NULL) {
        TABLE_ERROR("Error: Key not found\n");
        return NULL;
    }

    FOR_NODE(node, ks->node) {
        if (node->release == release) { return node; }
    }

    TABLE_ERROR("Error: Release not found\n");
    return NULL;
}

bool delete_one_version(Table *table, KeyType key, RelType release) {
    KeySpace *ks = search(table, key);
    if (ks == NULL) {
        TABLE_ERROR("Error: Key not found\n");
        return false;
    }

    Node *prev_node = NULL;
    FOR_NODE(node, ks->node) {
        if (node->release == release) {
            if (ks->node == node) {
                ks->node = node->next;
            } else {
                prev_node->next = node->next;
            }
            free(node->info);
            free(node);
            return true;
        }
        prev_node = node;
    }

    TABLE_ERROR("Error: Release not found\n");
    return false;
}

bool free_key_space_contents(Table *table, KeySpace *ks) {
    // printf("free ks %p %d %p %p\n", ks, ks->key, ks->next, ks->node);
    if (ks == NULL) {
        TABLE_ERROR("Error: Key not found\n");
        return false;
    }

    Node *prev_node = NULL;
    FOR_NODE(node, ks->node) {
        // printf("  free node %p\n", prev_node);
        if (prev_node) free(prev_node->info);
        free(prev_node);
        prev_node = node;
    }
    // printf("  free node %p\n", prev_node);
    if (prev_node) free(prev_node->info);
    free(prev_node);
    ks->node = NULL;
    return true;
}

bool delete_all_versions(Table *table, KeyType key) {
    return free_key_space_contents(table, search(table, key));
}

void output(Table *table) {
    printf("Table contents:\n");
    for (IndexType i = 0; i < table->msize; i++) {
        printf("table->ks[%zu]:\n", i);
        FOR_KEY_SPACE(ks, table->ks + i) {
            if (!ks->node) continue;
            printf("  [%u]:\n", ks->key);
            FOR_NODE(node, ks->node) {
                printf("    release %u = %u\n", node->release, *(node->info));
            }
        }
    }
}

bool import(Table *table, const char *filename) {
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        TABLE_ERROR("Error: File not found\n");
        return false;
    }

    // Insert the the table
    KeyType key;
    Item info;
    while (fscanf(file, "[%u] = %u", &key, &info) == 2) {
        if (!insert(table, key, info)) {
            fclose(file);
            return false;
        }
    }
    fclose(file);
    return true;
}

Node *search_all(Table *table, KeyType key) {
    static Node *node = NULL;

    if (table != NULL) {
        KeySpace *ks = search(table, key);
        if (ks == NULL) return NULL;
        node = ks->node;
        return node;
    }

    if (node == NULL) return NULL;
    node = node->next;
    return node;
}

Table *search_all_copy(Table *table, KeyType key) {
    Table *copy = create_table(table->msize);

    KeySpace *ks = search(table, key);
    FOR_NODE(node, ks->node) { insert(copy, key, *(node->info)); }

    return copy;
}

void free_table(Table *table) {
    if (table == NULL) return;
    // printf("free table %p\n", table);
    for (IndexType i = 0; i < table->msize; i++) {
        // printf("free index %zu\n", i);
        KeySpace *prev_ks = NULL;
        FOR_KEY_SPACE(ks, table->ks + i) {
            free_key_space_contents(table, ks);
            free(prev_ks);
            if (table->ks + i != ks) prev_ks = ks;
        }
        free(prev_ks);
    }
    free(table->ks);
    free(table);
}

bool dump_table(Table *table, const char *filename) {
    FILE *file = fopen(filename, "wb");
    if (file == NULL) {
        TABLE_ERROR("Error: File not found\n");
        return false;
    }

    fwrite(&(table->msize), sizeof(IndexType), 1, file);

    for (IndexType i = 0; i < table->msize; i++) {
        FOR_KEY_SPACE(ks, table->ks + i) {
            FOR_NODE(node, ks->node) {
                fwrite(&(ks->key), sizeof(KeyType), 1, file);
                fwrite(&(node->release), sizeof(RelType), 1, file);
                fwrite(node->info, sizeof(Item), 1, file);
            }
        }
    }
    fclose(file);
    return true;
}

Table *load_table(const char *filename) {
    FILE *file = fopen(filename, "rb");
    if (file == NULL) {
        TABLE_ERROR("Error: File not found\n");
        return NULL;
    }

    IndexType msize;
    fread(&msize, sizeof(IndexType), 1, file);
    Table *table = create_table(msize);

    KeyType key;
    RelType release;
    Item info;
    while (1) {
        if (!fread(&key, sizeof(KeyType), 1, file)) break;
        if (!fread(&release, sizeof(RelType), 1, file)) break;
        if (!fread(&info, sizeof(Item), 1, file)) break;
        insert_with_release(table, key, info, release);
    }
    fclose(file);
    return table;
}
