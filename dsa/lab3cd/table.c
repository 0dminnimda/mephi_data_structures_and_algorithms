#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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

Table *create_table(IndexType msize) {
    Table *table = calloc(1, sizeof(Table));
    table->ks = calloc(msize, sizeof(KeySpace));
    table->msize = msize;
    return table;
}

IndexType hash(Table *table, KeyType key) {
    return key % table->msize;
}

void insert(Table *table, KeyType key, RelType release, Item info) {
    IndexType index = hash(table, key);
    KeySpace *ks = table->ks + index;
    while (ks->next != NULL) {
        if (ks->key == key) {
            Node *node = ks->node;
            while (node && node->next != NULL) {
                node = node->next;
            }
            if (node && node->release == release) {
                *(node->info) = info;
                return;
            }
            Node *new_node = calloc(1, sizeof(Node));
            new_node->release = release;
            *(new_node->info) = info;
            new_node->next = NULL;
            if (node)
                node->next = new_node;
            else
                ks->node = new_node;
            return;
        }
        ks = ks->next;
    }
    KeySpace *new_ks = calloc(1, sizeof(KeySpace));
    Node *new_node = calloc(1, sizeof(Node));
    new_node->release = release;
    new_node->info = &info;
    new_node->next = NULL;
    new_ks->key = key;
    new_ks->node = new_node;
    new_ks->next = NULL;
    ks->next = new_ks;
}

Node *search(Table *table, KeyType key) {
    IndexType index = hash(table, key);
    KeySpace *ks = table->ks + index;
    while (ks->next != NULL) {
        ks = ks->next;
        if (ks->key == key) {
            return ks->node;
        }
    }
    return NULL;
}

void delete(Table *table, KeyType key, RelType release) {
    IndexType index = hash(table, key);
    KeySpace *ks = table->ks + index;
    KeySpace *prev_ks = ks;
    while (ks->next != NULL) {
        prev_ks = ks;
        ks = ks->next;
        if (ks->key == key && ks->release == release) {
            prev_ks->next = ks->next;
            free(ks->node);
            free(ks);
            return;
        }
    }
}

void delete_by_key(Table *table, KeyType key) {
    IndexType index = hash(table, key);
    KeySpace *ks = table->ks + index;
    KeySpace *prev_ks = ks;
    while (ks->next != NULL) {
        prev_ks = ks;
        ks = ks->next;
        if (ks->key == key) {
            prev_ks->next = ks->next;
            free(ks->node);
            free(ks);
        }
    }
}

void output(Table *table) {
    printf("Table contents:\n");
    for (IndexType i = 0; i < table->msize; i++) {
        for (KeySpace *ks = table->ks + i; ks->next != NULL; ks = ks->next) {
            printf("[%zu] - %u\n", i, ks->key);
            for (Node *node = ks->node; node && node->next; node = node->next)
                printf("  version %u = %u\n", ks->release, *(ks->node->info));
        }
    }
}

/*
void import(Table *table, const char *filename) {
    FILE *fp = fopen(filename, "r");
    if (fp == NULL) {
        printf("Error: could not open file %s\n", filename);
        exit(1);
    }
    char line[100];
    while (fgets(line, sizeof(line), fp) != NULL) {
        KeyType key;
        RelType release;
        Item *info = calloc(1, sizeof(Item));
        if (sscanf(line, "%u %u %u", &key, &release, info) != 3) {
            printf("Error: invalid input format in file %s\n", filename);
            exit(1);
        }
        insert(table, key, release, info);
    }
    fclose(fp);
}
*/

void fill_table(Table *table) {
    // create an array of prespecified keys and releases
    KeyType keys[] = { 1, 1, 1, 2, 2, 2, 2, 3, 1, 1 };
    Item values[] = { 11, 22, 33, 44, 55, 11, 11, 22, 66, 66 };
    IndexType numValues = sizeof(keys) / sizeof(keys[0]);

    for (int i = 0; i < numValues; i++) {
        insert(table, keys[i], 0, values[i]);
    }
}

Node *search_all(Table *table, KeyType key) {
    static KeySpace *ks = NULL;
    static Node *node = NULL;
    if (ks == NULL) {
        ks = table->ks;
        node = NULL;
    }
    while (ks - table->ks < table->msize) {
        if (node == NULL) {
            ks = ks->next;
            if (ks != NULL && ks->key == key) {
                node = ks->node;
                return node;
            }
        } else {
            node = node->next;
            if (node != NULL) {
                return node;
            } else {
                ks = ks->next;
                if (ks != NULL && ks->key == key) {
                    node = ks->node;
                    return node;
                }
            }
        }
    }
    return NULL;
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
