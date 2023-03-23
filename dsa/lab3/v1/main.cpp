// #include <vector>
// #include <iostream>

// using namespace std;

// struct KeySpace {
//     int key;
//     int par;
//     string info;
// };

// struct Item {
//     string info;
//     int key;
//     int ind;
//     KeySpace* p1;
// };

// struct Table {
//     vector<KeySpace> ks;
//     int msize;
// };

// class MyTable {
// public:
//     MyTable(int size) {
//         table.msize = size;
//     }

//     void addElement(int key, int par, string info) {
//         // Проверка, что ключ уникален
//         for (int i = 0; i < table.ks.size(); i++) {
//             if (table.ks[i].key == key) {
//                 cout << "Error: key already exists" << endl;
//                 return;
//             }
//         }
//         // Создание нового элемента
//         KeySpace newKs;
//         newKs.key = key;
//         newKs.par = par;
//         newKs.info = info;
//         table.ks.push_back(newKs);
//         // Обновление индексов элементов
//         for (int i = 0; i < table.ks.size(); i++) {
//             if (table.ks[i].key == par) {
//                 Item newItem;
//                 newItem.info = info;
//                 newItem.key = key;
//                 newItem.ind = table.ks.size() - 1;
//                 newItem.p1 = &table.ks[i];
//                 items.push_back(newItem);
//                 return;
//             }
//         }
//     }

//     void removeElement(int key) {
//         // Поиск элемента по ключу
//         int ind = -1;
//         for (int i = 0; i < table.ks.size(); i++) {
//             if (table.ks[i].key == key) {
//                 ind = i;
//                 break;
//             }
//         }
//         if (ind == -1) {
//             cout << "Error: element with key " << key << " doesn't exist" << endl;
//             return;
//         }
//         // Проверка, что нет элементов с таким же значением в поле ключа родителя
//         for (int i = 0; i < table.ks.size(); i++) {
//             if (table.ks[i].par == table.ks[ind].key) {
//                 cout << "Error: there are child elements with key " << table.ks[ind].key << endl;
//                 return;
//             }
//         }
//         // Удаление элемента
//         table.ks.erase(table.ks.begin() + ind);
//         for (int i = 0; i < items.size(); i++) {
//             if (items[i].key == key) {
//                 items.erase(items.begin() + i);
//                 return;
//             }
//         }
//     }

//     vector<Item> findElementsByParent(int par) {
//         vector<Item> result;
//         for (int i = 0; i < items.size(); i++) {
//             if (items[i].p1->key == par) {
//                 result.push_back(items[i]);
//             }
//         }
//         return result;
//     }

//     void printTable() {
//         for (int i = 0; i < table.ks.size(); i++) {
//             cout << "Key: " << table.ks[i].key << ", Parent Key: " << table.ks[i].par
//                 << ", Info: " << table.ks[i].info << endl;
//         }
//     }

// private:
//     Table table;
//     vector<Item> items;
// };

// int main() {
//     MyTable table(100);

//     table.addElement(1, 0, "Info 1");
//     table.addElement(2, 1, "Info 2");
//     table.addElement(3, 1, "Info 3");
//     table.addElement(4, 2, "Info 4");
//     table.addElement(5, 2, "Info 5");
//     table.addElement(6, 3, "Info 6");

//     table.printTable();

//     vector<Item> result = table.findElementsByParent(2);
//     cout << "Elements with parent key 2:" << endl;
//     for (int i = 0; i < result.size(); i++) {
//         cout << "Key: " << result[i].key << ", Info: " << result[i].info << endl;
//     }

//     table.removeElement(4);

//     table.printTable();

//     return 0;
// }

#include <iostream>
#include <fstream>
#include "TableMainMemory.h"
#include "TableExternalMemory.h"

using namespace std;

int main() {
    int type;
    cout << "Choose table type: (1) Main Memory or (2) External Memory" << endl;
    cin >> type;

    Table* table;

    if (type == 1) {
        table = new TableMainMemory();
    } else if (type == 2) {
        table = new TableExternalMemory();
    } else {
        cout << "Invalid table type" << endl;
        return 0;
    }

    int choice;
    do {
        cout << endl << "Choose operation: " << endl;
        cout << "1. Add element" << endl;
        cout << "2. Delete element" << endl;
        cout << "3. Search element" << endl;
        cout << "4. Print table" << endl;
        cout << "5. Import data from file" << endl;
        cout << "6. Exit" << endl;
        cin >> choice;

        switch (choice) {
            case 1: {
                cout << "Enter key value: ";
                KeyType key;
                cin >> key;

                cout << "Enter info value: ";
                InfoType* info = new InfoType();
                cin >> *info;

                Item* item = new Item();
                item->key = key;
                item->info = info;

                if (table->add(item)) {
                    cout << "Element added" << endl;
                } else {
                    cout << "Element with this key already exists" << endl;
                    delete item->info;
                    delete item;
                }
                break;
            }
            case 2: {
                cout << "Enter key value: ";
                KeyType key;
                cin >> key;

                if (table->remove(key)) {
                    cout << "Element with key " << key << " removed" << endl;
                } else {
                    cout << "Element with key " << key << " not found" << endl;
                }
                break;
            }
            case 3: {
                cout << "Enter key value: ";
                KeyType key;
                cin >> key;

                vector<Item*> items = table->search(key);
                if (items.empty()) {
                    cout << "Element with key " << key << " not found" << endl;
                } else {
                    cout << "Found " << items.size() << " element(s):" << endl;
                    for (auto item : items) {
                        cout << "Key: " << item->key << ", Info: " << *item->info << endl;
                    }
                }
                break;
            }
            case 4: {
                table->print();
                break;
            }
            case 5: {
                cout << "Enter filename: ";
                string filename;
                cin >> filename;

                ifstream file(filename);
                if (!file.is_open()) {
                    cout << "Error opening file" << endl;
                    break;
                }

                int n;
                file >> n;
                for (int i = 0; i < n; i++) {
                    KeyType key;
                    InfoType* info = new InfoType();

                    file >> key >> *info;

                    Item* item = new Item();
                    item->key = key;
                    item->info = info;

                    if (table->add(item)) {
                        cout << "Element added: Key: " << key << ", Info: " << *info << endl;
                    } else {
                        cout << "Element with key " << key << " already exists" << endl;
                        delete item->info;
                        delete item;
                    }
                }
                file.close();
                break;
            }
            case 6: {
                break;
            }
            default: {
                cout << "Invalid choice" << endl;
                break;
            }
        }
    } while (choice != 6);

    delete table;
    return 0;
}

/*
TableMainMemory myTable;
myTable.insert(1, "info1");
myTable.insert(2, "info2");
myTable.insert(1, "info3");
auto it = myTable.search(1);
Item* item;
while ((item = it.next()) != nullptr) {
std::cout << "key: " << item->key << ", info: " << item->info << std::endl;
}
*/

/*

#include <iostream>
#include <vector>
#include <algorithm>
#include <fstream>

// Structure for elements in a vector-based table
struct TableElement {
    unsigned int key;
    unsigned int parent_key;
    void* info;
};

// Structure for a vector-based table
struct Table {
    std::vector<TableElement> keyspace;
    unsigned int keyspace_size;
    unsigned int num_elements;
};

// Function to include a new element with unique key value
void includeElement(Table& table, unsigned int key, unsigned int parent_key, void* info) {
    if (std::find_if(table.keyspace.begin(), table.keyspace.end(),
    [&key](const TableElement& element) { return element.key == key; }) != table.keyspace.end()) {
        std::cerr << "Error: Element with key " << key << " already exists in table!" << std::endl;
        return;
    }

    TableElement element;
    element.key = key;
    element.parent_key = parent_key;
    element.info = info;
    table.keyspace.push_back(element);
    table.num_elements++;
}

// Function to delete an element by key value
void deleteElement(Table& table, unsigned int key) {
    auto it = std::find_if(table.keyspace.begin(), table.keyspace.end(),
    [&key](const TableElement& element) { return element.key == key; });
    if (it == table.keyspace.end()) {
        std::cerr << "Error: Element with key " << key << " not found in table!" << std::endl;
        return;
    }

    table.keyspace.erase(it);
    table.num_elements--;
}

// Function to search for elements by parent element key value
std::vector<TableElement> searchByParentKey(Table& table, unsigned int parent_key) {
    std::vector<TableElement> result;
    std::copy_if(table.keyspace.begin(), table.keyspace.end(), std::back_inserter(result),
    [&parent_key](const TableElement& element) { return element.parent_key == parent_key; });
    return result;
}

// Function to output table contents to standard stream
void outputTable(Table& table) {
    for (auto& element : table.keyspace) {
        std::cout << element.key << "\t" << element.parent_key << "\t" << element.info << std::endl;
    }
}

// Function to import data from a text file
void importData(Table& table, const char* filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Error: File " << filename << " could not be opened!" << std::endl;
        return;
    }

    while (!file.eof()) {
        unsigned int key, parent_key;
        void* info;
        file >> key >> parent_key >> info;
        includeElement(table, key, parent_key, info);
    }
    file.close();
}

int main() {
    Table table;
    table.keyspace_size = 0;
    table.num_elements = 0;

    // Add elements to table
    includeElement(table, 1, 0, (void*) 10);
    includeElement(table, 2, 0, (void*) 20);
    includeElement(table, 3, 1, (void*) 30);
    includeElement(table, 4, 2, (void*) 40);
    includeElement(table, 5, 2, (void*) 50);

    // Output table to standard stream
    outputTable(table);

    // Import data from text file
    importData(table, "data.txt");

    // Search for elements by parent element key
    std::vector<TableElement> elements = searchByParentKey(table, 2);
    for (auto& element : elements) {
        std::cout << element.key << "\t" << element.parent_key << "\t" << element.info << std::endl;
    }

    // Delete element with key
    deleteElement(table, 3);

    return 0;
}
*/

/*
#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>

using namespace std;

// Define the keyspace element structure
struct KeyspaceElement {
    unsigned int key;
    unsigned int parent_key;
    void* info;
};

// Define the table structure
struct Table {
    KeyspaceElement* keyspace;
    size_t keyspace_size;
    size_t num_elements;
};

// Function to add a new element to the table
void add_element(Table& table, unsigned int key, unsigned int parent_key, void* info) {
    // Check if the key already exists
    for (size_t i = 0; i < table.num_elements; i++) {
        if (table.keyspace[i].key == key) {
            cout << "Key already exists." << endl;
            return;
        }
    }
    // Resize the keyspace if necessary
    if (table.num_elements == table.keyspace_size) {
        table.keyspace_size *= 2;
        table.keyspace = (KeyspaceElement*)realloc(table.keyspace, table.keyspace_size * sizeof(KeyspaceElement));
    }
    // Add the new element to the keyspace
    table.keyspace[table.num_elements].key = key;
    table.keyspace[table.num_elements].parent_key = parent_key;
    table.keyspace[table.num_elements].info = info;
    table.num_elements++;
    // Sort the keyspace by parent_key
    sort(table.keyspace, table.keyspace + table.num_elements, [](const KeyspaceElement& a, const KeyspaceElement& b) {
        return a.parent_key < b.parent_key;
    });
}

// Function to delete an element from the table by key
void delete_element(Table& table, unsigned int key) {
    // Search for the element with the given key
    size_t index = 0;
    for (size_t i = 0; i < table.num_elements; i++) {
        if (table.keyspace[i].key == key) {
            index = i;
            break;
        }
    }
    // Shift the remaining elements in the keyspace
    for (size_t i = index; i < table.num_elements - 1; i++) {
        table.keyspace[i] = table.keyspace[i + 1];
    }
    // Decrement the number of elements and resize the keyspace if necessary
    table.num_elements--;
    if (table.num_elements <= table.keyspace_size / 2) {
        table.keyspace_size /= 2;
        table.keyspace = (KeyspaceElement*)realloc(table.keyspace, table.keyspace_size * sizeof(KeyspaceElement));
    }
}

// Function to search for elements by parent element key
vector<unsigned int> search_by_parent_key(Table& table, unsigned int parent_key) {
    vector<unsigned int> results;
    // Search for elements with the given parent key
    for (size_t i = 0; i < table.num_elements; i++) {
        if (table.keyspace[i].parent_key == parent_key) {
            results.push_back(table.keyspace[i].key);
        }
    }
    return results;
}

// Function to output the table contents to standard stream
void output_table(Table& table) {
    for (size_t i = 0; i < table.num_elements; i++) {
        cout << "Key: " << table.keyspace[i].key << ", Parent Key: " << table.keyspace[i].parent_key << endl;
    }
}

// Function to import data from a text file
void import_data(Table& table, const char* filename) {
    ifstream file(filename);
    if (!file.is_open()) {
        cout << "Error opening file." << endl;
        return;
    }
    unsigned int key, parent_key, value;
    while (file >> key >> parent_key >> value) {
        add_element(table, key, parent_key, (void*)&value);
    }
    file.close();
}

int main() {
    // Initialize the table in main memory
    Table main_memory_table;
    main_memory_table.keyspace_size = 16;
    main_memory_table.num_elements = 0;
    main_memory_table.keyspace = (KeyspaceElement*)malloc(main_memory_table.keyspace_size * sizeof(KeyspaceElement));

    // Test the table functions
    unsigned int key1 = 1;
    unsigned int parent_key1 = 0;
    unsigned int value1 = 10;
    add_element(main_memory_table, key1, parent_key1, (void*)&value1);

    unsigned int key2 = 2;
    unsigned int parent_key2 = 1;
    unsigned int value2 = 20;
    add_element(main_memory_table, key2, parent_key2, (void*)&value2);

    unsigned int key3 = 3;
    unsigned int parent_key3 = 1;
    unsigned int value3 = 30;
    add_element(main_memory_table, key3, parent_key3, (void*)&value3);

    vector<unsigned int> search_results = search_by_parent_key(main_memory_table, 1);
    for (unsigned int key : search_results) {
        cout << "Search Result: " << key << endl;
    }

    delete_element(main_memory_table, 2);

    output_table(main_memory_table);

    import_data(main_memory_table, "data.txt");

    // Cleanup the table in main memory
    for (size_t i = 0; i < main_memory_table.num_elements; i++) {
        free(main_memory_table.keyspace[i].info);
    }
    free(main_memory_table.keyspace);

    return 0;
}
*/
