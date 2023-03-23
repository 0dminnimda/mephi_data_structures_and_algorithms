#include <iostream>
#include <vector>
#include <string>
#include "table_mem.h"
#include "table_file.h"

using namespace std;

int main() {
    int choice;
    Table *table = nullptr;
    string filename;

    while (true) {
        cout << "Please choose the table type:" << endl;
        cout << "1. Main memory" << endl;
        cout << "2. External memory" << endl;
        cout << "3. Quit" << endl;
        cout << "Choice: ";
        cin >> choice;

        if (choice == 3) {
            break;
        }

        if (choice == 2) {
            cout << "Please enter the filename: ";
            cin >> filename;
            table = new TableFile(filename);
        } else {
            table = new TableMem();
        }

        while (true) {
            cout << "Please choose the operation:" << endl;
            cout << "1. Add element" << endl;
            cout << "2. Delete element" << endl;
            cout << "3. Search elements by parent key" << endl;
            cout << "4. Output table contents" << endl;
            cout << "5. Import data from text file" << endl;
            cout << "6. Special operation" << endl;
            cout << "7. Switch table type" << endl;
            cout << "8. Quit" << endl;
            cout << "Choice: ";
            cin >> choice;

            if (choice == 8) {
                break;
            }

            switch (choice) {
                case 1: {
                    unsigned int key, parent_key, value;
                    cout << "Please enter the key, parent key, and value: ";
                    cin >> key >> parent_key >> value;
                    bool result = table->add_element(key, parent_key, value);
                    if (!result) {
                        cout << "Failed to add element. Key already exists." << endl;
                    }
                    break;
                }
                case 2: {
                    unsigned int key;
                    cout << "Please enter the key: ";
                    cin >> key;
                    bool result = table->delete_element(key);
                    if (!result) {
                        cout << "Failed to delete element. Key does not exist." << endl;
                    }
                    break;
                }
                case 3: {
                    unsigned int parent_key;
                    cout << "Please enter the parent key: ";
                    cin >> parent_key;
                    vector<Element*> elements = table->search_by_parent_key(parent_key);
                    for (Element *e : elements) {
                        cout << "Key: " << e->key << ", Parent key: " << e->parent_key << ", Value: " << e->value << endl;
                    }
                    break;
                }
                case 4: {
                    table->output();
                    break;
                }
                case 5: {
                    string filename;
                    cout << "Please enter the filename: ";
                    cin >> filename;
                    bool result = table->import_data(filename);
                    if (!result) {
                        cout << "Failed to import data." << endl;
                    }
                    break;
                }
                case 6: {
                    // Special operation
                    break;
                }
                case 7: {
                    if (table->is_file()) {
                        delete table;
                        table = new TableMem();
                    } else {
                        delete table;
                        cout << "Please enter the filename: ";
                        cin >> filename;
                        table = new TableFile(filename);
                    }
                    break;
                }
                default: {
                    cout << "Invalid choice." << endl;
                    break;
                }
            }
        }

        delete table;
    }

    return 0;
}
