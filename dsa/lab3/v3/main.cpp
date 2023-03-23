
#include <fstream>
#include <iostream>

#define TABLE_TYPE 'm'

// clang-format off
#if TABLE_TYPE == 'm'
    #include "table_mem.h"
    using ChosenTable = TableMem;
#elif TABLE_TYPE == 'e'
    #include "table_ext.h"
    using ChosenTable = TableExt;
#else
    #error Invalid table type
#endif
// clang-format on

int main() {
    std::size_t size;
    std::cout << "Input table size: ";
    std::cin >> size;

    Table* table = new ChosenTable(size);

    int choice;
    do {
        std::cout << std::endl << "Choose operation: " << std::endl;
        std::cout << "1. Add element" << std::endl;
        std::cout << "2. Delete element" << std::endl;
        std::cout << "3. Search element" << std::endl;
        std::cout << "4. Print table" << std::endl;
        std::cout << "5. Import data from file" << std::endl;
        std::cout << "6. Exit" << std::endl;
        std::cin >> choice;

        switch (choice) {
            case 1: {
                std::cout << "Enter key value: ";
                KeyType key;
                std::cin >> key;

                std::cout << "Enter info value: ";
                InfoType* info = new InfoType();
                std::cin >> *info;

                Item* item = new Item();
                item->key = key;
                item->info = info;

                if (table->add_element(key, item)) {
                    std::cout << "Element added" << std::endl;
                } else {
                    std::cout << "Element with this key already exists" << std::endl;
                    delete item->info;
                    delete item;
                }
                break;
            }
            case 2: {
                std::cout << "Enter key value: ";
                KeyType key;
                std::cin >> key;

                if (table->delete_element(key)) {
                    std::cout << "Element with key " << key << " removed" << std::endl;
                } else {
                    std::cout << "Element with key " << key << " not found" << std::endl;
                }
                break;
            }
            case 3: {
                std::cout << "Enter key value: ";
                KeyType key;
                std::cin >> key;

                vector<Item*> items = table->search(key);
                if (items.empty()) {
                    std::cout << "Element with key " << key << " not found" << std::endl;
                } else {
                    std::cout << "Found " << items.size() << " element(s):" << std::endl;
                    for (auto item : items) {
                        std::cout << "Key: " << item->key << ", Info: " << *item->info << std::endl;
                    }
                }
                break;
            }
            case 4: {
                table->print();
                break;
            }
            case 5: {
                std::cout << "Enter filename: ";
                string filename;
                std::cin >> filename;

                ifstream file(filename);
                if (!file.is_open()) {
                    std::cout << "Error opening file" << std::endl;
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
                        std::cout << "Element added: Key: " << key << ", Info: " << *info
                                  << std::endl;
                    } else {
                        std::cout << "Element with key " << key << " already exists" << std::endl;
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
                std::cout << "Invalid choice" << std::endl;
                break;
            }
        }
    } while (choice != 6);

    delete table;
    return 0;
}

/*
Design and implement in C++:

- Library for vector-based table
- Console application to work with library table interactively (Model View Controller)

Implement library:

1. In-memory
2. External memory (binary file)

Vector-based table:

```c
typedef unsigned int KeyType;
typedef unsigned int InfoType;

struct Item {
    InfoType *info; // actual data
    KeyType key; // key for item's key space
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
- Search by key (return copies of matches)
- Output to standard stream
- Import from text file
- Remove by key if no dup parent keys
- Search by parent key value, return new table of matches

Additional tasks (extra points):

1. Search result as iterator
2. All ops in external memory (only table in main)
3. Buffering/caching for external memory

Requirements:

- No globals
- Check input, show errors
- Show errors for table ops, continue
- Modified struct for external memory (add length/offset)
- Use fread/fwrite for files (with length)
- Use build system (Make/CMake)
- Build libraries/app independently
- Correct memory usage (valgrind, sanitizers, IDE tools)
*/






/*
Design and implement program in C++:
Write program in C++:
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
- Remove by key if no dup parent keys
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

/*
I just got an idea of merging two or more shared variables into one after the if statement
I know that in "true" block I use shared x, y, z for my variable and in "false" block I used a, b, c, then after the block I can use the latest variable that contains nonzero pointer
This can work because only of the block blocks will be actually executed
I't also important to set the previous shared to null pointer 
*/

/*
Here is a sample implementation of the library for vector-based table in C++:

```c++
#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>

using KeyType = unsigned int;
using InfoType = unsigned int;

class Item {
public:
    InfoType *info; // actual data
    KeyType key; // key for item's key space
};

class KeySpace {
public:
    KeyType key;
    KeyType par; // nullable
    Item *info;
    size_t length; // for external memory only
    size_t offset; // for external memory only
};

class Table {
public:
    KeySpace *ks;
    size_t msize; // max key space size
};

class VectorTable {
private:
    Table table;
    std::vector<Item> items;
public:
    VectorTable(size_t max_size) {
        table.ks = new KeySpace[max_size];
        table.msize = max_size;
    }

    ~VectorTable() {
        delete[] table.ks;
    }

    bool insert(const Item& item);

    bool remove(KeyType key);

    Item* find(KeyType key);

    std::vector<Item*> find_by_parent(KeyType key);

    void output(std::ostream& os) const;

    bool import(const std::string& filename);
};

class ExternalTable {
private:
    Table table;
    std::string filename;
    std::fstream file;
public:
    ExternalTable(const std::string& filename, size_t max_size);

    ~ExternalTable();

    bool insert(const Item& item);

    bool remove(KeyType key);

    Item* find(KeyType key);

    std::vector<Item*> find_by_parent(KeyType key);

    void output(std::ostream& os) const;

    bool import(const std::string& filename);

    bool flush();
};

bool VectorTable::insert(const Item& item) {
    // TODO: implement
    return false;
}

bool VectorTable::remove(KeyType key) {
    // TODO: implement
    return false;
}

Item* VectorTable::find(KeyType key) {
    // TODO: implement
    return nullptr;
}

std::vector<Item*> VectorTable::find_by_parent(KeyType key) {
    // TODO: implement
    return {};
}

void VectorTable::output(std::ostream& os) const {
    // TODO: implement
}

bool VectorTable::import(const std::string& filename) {
    // TODO: implement
    return false;
}

ExternalTable::ExternalTable(const std::string& filename, size_t max_size) {
    // TODO: implement
}

ExternalTable::~ExternalTable() {
    // TODO: implement
}

bool ExternalTable::insert(const Item& item) {
    // TODO: implement
    return false;
}

bool ExternalTable::remove(KeyType key) {
    // TODO: implement
    return false;
}

Item* ExternalTable::find(KeyType key) {
    // TODO: implement
    return nullptr;
}

std::vector<Item*> ExternalTable::find_by_parent(KeyType key) {
    // TODO: implement
    return {};
}

void ExternalTable::output(std::ostream& os) const {
    // TODO: implement
}

bool ExternalTable::import(const std::string& filename) {
    // TODO: implement
    return false;
}

bool ExternalTable::flush() {
    // TODO: implement
    return false;
}
```

The library provides the following operations:

- `insert()` - inserts an element with a unique key into the table
- `remove()` - removes an element by the key from the table
- `find()` - searches for an element by the key and returns a pointer to the item
- `find_by_parent()` - searches for elements by the parent key and returns a vector of pointers to the items
- `output()` - outputs the entire table to the specified stream
- `import()` - imports the table from a text file
- `flush()` - writes any changes to the external file

The library has two implementations: `VectorTable` for in-memory storage and `ExternalTable` for external file storage. The `Table` class is used to store the key space information and the maximum size of the table.

Each implementation of the table operations is left blank and will need to be implemented according to the specific storage requirements. The `flush()` method is used only for the external table implementation and is responsible for writing any changes to the external file.

The sample implementation follows the MVC design pattern, with the library being the model and the console application being the view and controller. The console application will interact with the library to perform the table operations and display the results to the user.
*/
