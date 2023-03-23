/*
Write Library for vector-based table in C++:

Library implementations: In-memory

`table.hpp`:
```c
#include <iostream>
#include <memory>
#include <vector>

using KeyType = unsigned int;
using InfoType = unsigned int;
using IndexType = std::size_t;

class Item {
public:
    std::unique_ptr<InfoType> info;  // actual data
    KeyType key;                     // backreference to KeySpace

    Item(KeyType k, InfoType* i) : key(k), info(i) {}
};

class KeySpace {
public:
    KeyType key;
    KeyType par;  // nullable
    std::unique_ptr<Item> info;

    std::size_t length;  // for external memory only
    std::size_t offset;  // for external memory only

    KeySpace(KeyType key, KeyType parent_key, InfoType* info)
        : key(key), par(parent_key), info(new Item(key, info)) {}
};

class Table {
public:
    std::vector<KeySpace> ks;
    IndexType msize;  // max key space size

    Table(IndexType msize) : msize(msize) {}
    virtual ~Table() = default;

    virtual bool insert(KeyType key, InfoType* info) = 0;
    virtual bool remove(KeyType key) = 0;
    virtual SearchIterator search(KeyType key) = 0;
    virtual void output(std::ostream& out) = 0;
    virtual bool import(const std::string& filename) = 0;
    virtual bool removeByParentKey(KeyType parent_key) = 0;
    virtual Table* searchByParentKey(KeyType parent_key) = 0;
};

class SearchIterator {
public:
    virtual ~SearchIterator();

    virtual Item& operator*();
    virtual SearchIterator& operator++();
    virtual bool operator!=(const SearchIterator& other) const;
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

// #include "table_mem.hpp"

// class InMemorySearchIterator : public SearchIterator {

// };

// class InMemoryTable : public Table {
// public:
//     bool insert(KeyType k, InfoType* i) override;
//     bool remove(KeyType k) override;
//     SearchIterator search(KeyType k) override;
//     void output(std::ostream& out) override;
//     bool import(const std::string& filename) override;
//     bool removeByParentKey(KeyType pk) override;
//     Table* searchByParentKey(KeyType pk) override;
// };

