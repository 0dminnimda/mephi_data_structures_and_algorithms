#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define TABLE_PRINT_ERRORS

#include "common/input.h"
#include "table.h"

void printMenu();
void printTable(Table *table);
void printItem(Item *item);

void insertCommand(Table *table) {
    printf("Enter key, parent key, and info (separated by spaces): ");
    char *input = read_line();

    // Parse input and insert item into table
    KeyType key, parKey;
    InfoType info;
    int count = sscanf(input, "%u %u %u", &key, &parKey, &info);
    if (count == 3) {
        bool success = insertItem(table, key, parKey, info);
        if (!success) {
            printf("Error: Failed to insert item into table\n");
        }
    } else {
        printf("Error: Invalid input format\n");
    }

    free(input);
}

void removeByKeyCommand(Table *table) {
    printf("Enter key to delete: ");
    char *input = read_line();

    // Parse input and remove item from table
    KeyType key;
    int count = sscanf(input, "%u", &key);
    if (count == 1) {
        bool success = deleteItem(table, key);
        if (!success) {
            printf("Error: Failed to delete item from table\n");
        }
    } else {
        printf("Error: Invalid input format\n");
    }

    free(input);
}

void findFirstPlaceByParentCommand(Table *table) {
    printf("Enter parent key to search for: ");
    char *input = read_line();

    // Parse input and search for item in table
    KeyType parKey;
    int count = sscanf(input, "%u", &parKey);
    if (count == 1) {
        IndexType index = findFirstPlaceByParent(table, parKey);
        printf("Item found (at %lu):\n", index);
        printItem(table->ks[index].info);
    } else {
        printf("Error: Invalid input format\n");
    }

    free(input);
}

void searchByKeyCommand(Table *table) {
    printf("Enter key to search for: ");
    char *input = read_line();

    // Parse input and search for item in table
    KeyType key;
    int count = sscanf(input, "%u", &key);
    if (count == 1) {
        Item *item = searchByKey(table, key);
        if (item != NULL) {
            printf("Item found:\n");
            printItem(item);
        } else {
            printf("Item not found\n");
        }
    } else {
        printf("Error: Invalid input format\n");
    }

    free(input);
}

void outputCommand(Table *table) { printTable(table); }

void importCommand(Table *table) {
    printf("Enter filename to import from: ");
    char *input = read_line();

    // Import table from file
    bool success = importTable(table, input);
    if (!success) {
        printf("Error: Failed to import table from file\n");
    }

    free(input);
}

void removeByKeyIfNotParentCommand(Table *table) {
    printf("Enter key to remove if no parent keys in table = key: ");
    char *input = read_line();

    // Parse input and remove item from table if no parent keys in table = key
    KeyType key;
    int count = sscanf(input, "%u", &key);
    if (count == 1) {
        bool success = removeByKeyIfNotParent(table, key);
        if (!success) {
            printf("Error: Failed to remove item from table\n");
        }
    } else {
        printf("Error: Invalid input format\n");
    }

    free(input);
}

void searchByParentKeyCommand(Table *table) {
    printf("Enter parent key to search for: ");
    char *input = read_line();

    // Parse input and search for items with matching parent key in table
    KeyType parKey;
    int count = sscanf(input, "%u", &parKey);
    if (count == 1) {
        Table *newTable = searchByParentKey(table, parKey);
        if (newTable != NULL) {
            printf("Items with parent key %u:\n", parKey);
            printTable(newTable);
            destroyTable(newTable);
        } else {
            printf("No items with parent key %u\n", parKey);
        }
    } else {
        printf("Error: Invalid input format\n");
    }

    free(input);
}

void printMenu() {
    printf("Menu:\n");
    printf("i   Insert element\n");
    // printf("d   Delete element\n");
    printf("s   Search by key\n");
    printf("o   Output table\n");
    printf("f   Import from file\n");
    printf("dnp Remove item by key if no parent keys in table = key\n");
    printf("sp  Search items by parent key\n");
    printf("h   Print menu\n");
    printf("q   Quit\n");
}

void printTable(Table *table) {
    outputTable(table);
    // printf("Table:\n");
    // printf("Index\tKey\tParent Key\tInfo\n");
    // for (IndexType i = 0; i < table->msize; i++) {
    //     printf("%lu\t%u\t%u\t\t%u\n", i, table->ks[i].key, table->ks[i].par,
    //            table->ks[i].info->info);
    // }
}

void printItem(Item *item) { printf("Key: %u\tInfo: %u\n", item->key, *(item->info)); }

int main() {
    Table *table = createTable(10);
    printMenu();

    bool run = true;
    while (run) {
        printf("> ");
        char *input = read_line();

        if (strcmp(input, "i") == 0) {
            insertCommand(table);
        // } else if (strcmp(input, "d") == 0) {
        //     removeByKeyCommand(table);
        } else if (strcmp(input, "ssp") == 0) {
            findFirstPlaceByParentCommand(table);
        } else if (strcmp(input, "s") == 0) {
            searchByKeyCommand(table);
        } else if (strcmp(input, "o") == 0) {
            outputCommand(table);
        } else if (strcmp(input, "f") == 0) {
            importCommand(table);
        } else if (strcmp(input, "dnp") == 0) {
            removeByKeyIfNotParentCommand(table);
        } else if (strcmp(input, "sp") == 0) {
            searchByParentKeyCommand(table);
        } else if (strcmp(input, "h") == 0) {
            printMenu();
        } else if (strcmp(input, "q") == 0) {
            run = false;
        } else {
            printf("Error: Invalid input\n");
        }

        free(input);
    }

    destroyTable(table);
    return 0;
}
