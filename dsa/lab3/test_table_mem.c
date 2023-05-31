#include <stdbool.h>

// #define NO_TABLE_PRINT_ERRORS

#include "table_mem.c"

bool test_basic() {
    bool test_result = true;
    Table *table = createTable(10);
    insertItem(table, 1, 0, 100);
    insertItem(table, 2, 0, 200);
    insertItem(table, 3, 0, 300);
    outputTable(table), printf("\n");
    Item *item1 = searchByKey(table, 1);
    Item *item2 = searchByKey(table, 2);
    Item *item3 = searchByKey(table, 3);
    if (*(item1->info) != 100 || *(item2->info) != 200 || *(item3->info) != 300) {
        printf("Failed: insert or search functionality not working\n");
        test_result = false;
    }
    destroyTable(table);
    return test_result;
}

bool test_parent_search() {
    bool test_result = true;
    Table *table = createTable(10);
    insertItem(table, 1, 0, 100);
    insertItem(table, 2, 1, 200);
    insertItem(table, 3, 1, 300);
    outputTable(table), printf("\n");
    Table *subTable = searchByParentKey(table, 1);
    outputTable(table), printf("\n");
    outputTable(subTable), printf("\n");
    Item *item2 = searchByKey(subTable, 2);
    Item *item3 = searchByKey(subTable, 3);
    if (*(item2->info) != 200 || *(item3->info) != 300) {
        printf("Failed: inserting items with parent keys not working\n");
        test_result = false;
    }
    destroyTable(table);
    destroyTable(subTable);
    return test_result;
}

bool test_deleting() {
    bool test_result = true;
    Table *table = createTable(10);
    insertItem(table, 1, 0, 100);
    insertItem(table, 2, 0, 200);
    insertItem(table, 3, 0, 300);
    outputTable(table), printf("\n");
    deleteItem(table, 2);
    outputTable(table), printf("\n");
    Item *item1 = searchByKey(table, 1);
    Item *item2 = searchByKey(table, 2);
    Item *item3 = searchByKey(table, 3);
    if (item2 != NULL || *(item1->info) != 100 || *(item3->info) != 300) {
        printf("Failed: deleting items not working\n");
        printf("%d %d %d\n", item2 != NULL, *(item1->info) != 100, *(item3->info) != 300);
        test_result = false;
    }
    destroyTable(table);
    return test_result;
}

bool test_deleting_absent() {
    bool test_result = true;
    Table *table = createTable(10);
    insertItem(table, 1, 0, 100);
    outputTable(table), printf("\n");
    bool result = deleteItem(table, 2);
    outputTable(table), printf("\n");
    if (result) {
        printf("Failed: deleting non-existent item returned true\n");
        test_result = false;
    }
    destroyTable(table);
    return test_result;
}

bool test_importing() {
    bool test_result = true;
    Table *table = createTable(10);
    outputTable(table), printf("\n");
    bool result = importTable(table, "testdata.txt");
    outputTable(table), printf("\n");
    Item *item1 = searchByKey(table, 1);
    Item *item2 = searchByKey(table, 2);
    Item *item3 = searchByKey(table, 3);
    if (!result || *(item1->info) != 100 || *(item2->info) != 200 ||
        *(item3->info) != 300) {
        printf("Failed: importing data from file not working\n");
        test_result = false;
    }
    destroyTable(table);
    return test_result;
}

bool test_removing_if_not_parent() {
    bool test_result = true;
    Table *table = createTable(10);
    insertItem(table, 1, 0, 100);
    insertItem(table, 2, 1, 200);
    insertItem(table, 3, 2, 300);
    outputTable(table), printf("\n");
    bool result = removeByKeyIfNotParent(table, 3);
    outputTable(table), printf("\n");
    Item *item1 = searchByKey(table, 1);
    Item *item2 = searchByKey(table, 2);
    Item *item3 = searchByKey(table, 3);
    if (!result || item1 == NULL || item2 == NULL || item3 != NULL) {
        printf("Failed: removing items if it is not parent not working - ");
        printf("%d %d %d %d\n", !result, item2 != NULL, item3 != NULL, item1 != NULL);
        test_result = false;
    }
    destroyTable(table);
    return test_result;
}

bool test_removing_if_parent() {
    bool test_result = true;
    Table *table = createTable(10);
    insertItem(table, 1, 0, 100);
    insertItem(table, 2, 1, 200);
    insertItem(table, 3, 2, 300);
    outputTable(table), printf("\n");
    bool result = removeByKeyIfNotParent(table, 1);
    outputTable(table), printf("\n");
    Item *item1 = searchByKey(table, 1);
    Item *item2 = searchByKey(table, 2);
    Item *item3 = searchByKey(table, 3);
    if (result || item1 == NULL || item2 == NULL || item3 == NULL) {
        printf("Failed: removing items if it is parent returned true\n");
        test_result = false;
    }
    destroyTable(table);
    return test_result;
}

bool test_invalid_size() {
    bool test_result = true;
    Table *table = createTable(0);
    if (table != NULL) {
        printf("Failed: creating table with invalid size did not return NULL\n");
        test_result = false;
    }
    destroyTable(table);
    return test_result;
}

bool test_importing_absent() {
    bool test_result = true;
    Table *table = createTable(10);
    bool result = importTable(table, "nonexistent.txt");
    if (result) {
        printf("Failed: importing data from non-existent file returned true\n");
        test_result = false;
    }
    destroyTable(table);
    return test_result;
}

bool test_inserting_in_full() {
    bool test_result = true;
    Table *table = createTable(3);
    insertItem(table, 1, 0, 100);
    insertItem(table, 2, 0, 200);
    insertItem(table, 3, 0, 300);
    outputTable(table), printf("\n");
    bool result = insertItem(table, 4, 0, 400);
    outputTable(table), printf("\n");
    if (result) {
        printf("Failed: inserting item when table is full returned true\n");
        test_result = false;
    }
    destroyTable(table);
    return test_result;
}

int main() {
    bool (*test_functions[])() = {
        test_basic,
        test_parent_search,
        test_deleting,
        test_deleting_absent,
        test_importing,
        test_removing_if_not_parent,
        test_removing_if_parent,
        // test_invalid_size,
        test_importing_absent,
        test_inserting_in_full,
    };

    size_t len = sizeof(test_functions) / sizeof(test_functions[0]);

    bool success = true;
    for (int i = 0; i < len; i++) {
        printf("\n\nTest %d\n", i + 1);
        bool result = test_functions[i]();
        if (result) {
            printf("Succeeded\n");
        }
        success = success && result;
    }
    return (int)(!success);
}
