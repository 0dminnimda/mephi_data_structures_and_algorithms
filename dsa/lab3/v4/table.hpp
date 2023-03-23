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

    virtual bool insert(KeyType key, InfoType* info, KeyType parent_key) = 0;
    virtual bool remove(KeyType key) = 0;
    virtual SearchIterator search(KeyType key) = 0;
    virtual void output(std::ostream& out) const = 0;
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
    virtual SearchIterator& end() const;
};
