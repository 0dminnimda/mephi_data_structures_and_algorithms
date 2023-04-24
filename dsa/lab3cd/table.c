#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

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
    for (KeySpace *name = init; name->next != NULL; name = name->next)

#define FOR_NODE(name, init) \
    for (Node *name = init; name != NULL && name->next != NULL; name = name->next)

Table *create_table(IndexType msize) {
    Table *table = calloc(1, sizeof(Table));
    table->ks = calloc(msize, sizeof(KeySpace));
    table->msize = msize;
    return table;
}

IndexType hash(Table *table, KeyType key) {
    return key % table->msize;
}

bool insert(Table *table, KeyType key, Item info) {
    IndexType index = hash(table, key);
    KeySpace *target_ks = NULL;
    KeySpace *prev_ks = NULL;

    FOR_KEY_SPACE(ks, table->ks + index) {
        if (ks->key == key) {
            target_ks = ks;
            break;
        }
        prev_ks = ks;
    }

    if (!target_ks) {
        target_ks = calloc(1, sizeof(KeySpace));
        target_ks->key = key;
    }
    assert(prev_ks);
    prev_ks->next = target_ks;

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

    // Node *last_node = NULL;
    // FOR_NODE(node, target_ks->node) last_node = node;

    // Node *new_node = calloc(1, sizeof(Node));
    // new_node->release = last_node? last_node->release + 1 : 1;
    // new_node->info = calloc(1, sizeof(Item));
    // *(new_node->info) = info;
    // new_node->next = NULL;

    // if (last_node)
    //     last_node->next = new_node;
    // else
    //     target_ks->node = new_node;

    return true;
}

KeySpace *search(Table *table, KeyType key) {
    IndexType index = hash(table, key);
    FOR_KEY_SPACE(ks, (table->ks + index)) {
        if (ks->key == key) {
            return ks;
        }
    }
    return NULL;
}

bool delete_one_version(Table *table, KeyType key, RelType release) {
    KeySpace *ks = search(table, key);
    if (ks == NULL) return false;

    Node *prev_node = NULL;
    FOR_NODE(node, ks->node) {
        if (node->release == release) {
            if (ks->node == node) {
                ks->node = NULL;
            } else {
                prev_node->next = node->next;
            }
            free(node->info);
            free(node);
            return true;
        }
        prev_node = node;
    }

    return false;
}

bool delete_all_versions(Table *table, KeyType key) {
    KeySpace *ks = search(table, key);
    if (ks == NULL) return false;

    Node *prev_node = NULL;
    FOR_NODE(node, ks->node) {
        if (prev_node) free(prev_node->info);
        free(prev_node);
        prev_node = node;
    }
    if (prev_node) free(prev_node->info);
    free(prev_node);
    return true;
}

void output(Table *table) {
    printf("Table contents:\n");
    for (IndexType i = 0; i < table->msize; i++) {
        FOR_KEY_SPACE(ks, table->ks + i) {
            printf("[%zu] -> %u:\n", i, ks->key);
            FOR_NODE(node, ks->node)
                printf("  version %u = %u\n", node->release, *(node->info));
        }
    }
}

// void import(Table *table, const char *filename) {
//     FILE *fp = fopen(filename, "r");
//     if (fp == NULL) {
//         printf("Error: could not open file %s\n", filename);
//         exit(1);
//     }
//     char line[100];
//     while (fgets(line, sizeof(line), fp) != NULL) {
//         KeyType key;
//         RelType release;
//         Item *info = calloc(1, sizeof(Item));
//         if (sscanf(line, "%u %u %u", &key, &release, info) != 3) {
//             printf("Error: invalid input format in file %s\n", filename);
//             exit(1);
//         }
//         insert(table, key, release, info);
//     }
//     fclose(fp);
// }

#define TABLE_ERROR(...) "popo"

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

void fill_table(Table *table) {
    // create an array of prespecified keys and releases
    KeyType keys[] = { 1, 1, 1, 2, 2, 2, 2, 3, 1, 1 };
    Item values[] = { 11, 22, 33, 44, 55, 11, 11, 22, 66, 66 };
    IndexType numValues = sizeof(keys) / sizeof(keys[0]);

    for (int i = 0; i < numValues; i++) {
        insert(table, keys[i], values[i]);
    }
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

void free_table(Table *table) {
    for (IndexType i = 0; i < table->msize; i++) {
        KeySpace *ks = table->ks + i;
        while (ks->next != NULL) {
            KeySpace *next_ks = ks->next;
            Node *node = ks->node;
            while (node != NULL) {
                Node *next_node = node->next;
                free(node->info);
                free(node);
                node = next_node;
            }
            free(ks);
            ks = next_ks;
        }
    }
    free(table->ks);
    free(table);
}
