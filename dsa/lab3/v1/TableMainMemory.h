#include <vector>

typedef int KeyType;
typedef std::string InfoType;
typedef int IndexType;
typedef int PointerType;

struct Item {
    InfoType info;
    KeyType key;
};

class TableMainMemory {
  public:
    TableMainMemory();
    ~TableMainMemory();

    bool add(const Item &item);
    bool remove(KeyType key);
    std::vector<Item> search(KeyType key);
    // Iterator search(KeyType key);
    void print();

    class Iterator;

  private:
    std::vector<Item> items;
};

class TableMainMemory::Iterator {
  public:
    Iterator(std::vector<Item> &items, KeyType key);

    Item *next();

  private:
    std::vector<Item> &items;
    KeyType key;
    int index;
};


/*
Write in C++:
Outline an implementation for program in C++:
Formulate a step-by-step guide for how to implement this program in C++:
Describe in detail the implementation for this program in C++:

1. An interactive application program for working with a vector-based table.
2. Two libraries for working with a table - one for tables located in main memory and one for tables in external memory.
3. The table is defined by a structure with a keyspace pointer, keyspace size, and current number of elements. Keyspace elements are defined by a structure with a non-null element key, parent element key, and pointer to information.
4. The operations to be provided are: inclusion of a new element with unique key value, deletion of an element by key value, deletion of an element from the table using its specified key, only if there are no other elements in the table with a matching parent key value, search for elements by key value, search for all elements with a parent key value and create a new table with the results, output of table contents to standard stream, import of data from a text file.
5. Two solutions are required - one with table and element information stored in main memory, and the other with information stored in external memory using a binary random access file.
6. Additional tasks for extra points include implementing search as an iterator, performing all operations with external memory keyspaces, and implementing buffering and caching of file operations.
7. Functions should be designed as separate logical parts with parameters, and global variables are not allowed. Functions for working with tables should not be interactive, and should follow the Model-View-Controller principle. The program should check for errors in input data and display appropriate messages.
8. For tables in external memory, a modified structure with information length and offset in the file should be used. The fread() and fwrite() functions should be used with the actual length of information as an argument.
9. A build system such as Make or CMake must be used to build the program and libraries, which must be built independently of each other.
11. The vector-based table has a key and parent element key, and the elements are ordered by the value of the parent element's key. Special operations include removal of an element with a matching key and search for all elements with a matching parent element key. Key is an unsigned integer and value is an unsigned integer.

Here are the structures:

```c
struct Item {
    // pointer to information
    InfoType *info;
    // optional fields, to optimize the execution of operations,
    // composition and presence of which must be justified:
    // element key
    KetType key;
    // link to keyspace element by index
    IndexType ind;
    // link to keyspace element by pointer
    PointerType *p1;
};

struct KeySpace {
     // non-null element key
     keyType key;
     // parent element key, can be null
     KeyType par;
     // pointer to information
     Item *info;
};

struct Table {
    // pointer to keyspace
    KeySpace *ks;
    // field that limits the size of the key space
    // keyspace region size
    IndexType msize;
};
```
*/

