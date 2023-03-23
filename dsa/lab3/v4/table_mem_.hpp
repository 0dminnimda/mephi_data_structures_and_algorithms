#include "table.hpp"

class InMemoryTable : public Table {
public:
    InMemoryTable(IndexType msize) : Table(msize) {}

    bool insert(KeyType key, InfoType* info) override;
    bool remove(KeyType key) override;
    SearchIterator search(KeyType key) override;
    void output(std::ostream& out) override;
    bool import(const std::string& filename) override;
    bool removeByParentKey(KeyType parent_key) override;
    Table* searchByParentKey(KeyType parent_key) override;
};

bool InMemoryTable::insert(KeyType key, InfoType* info) {
    // TODO: Implement insert method
    return false;
}

bool InMemoryTable::remove(KeyType key) {
    // TODO: Implement remove method
    return false;
}

SearchIterator InMemoryTable::search(KeyType key) {
    InMemorySearchIterator iterator;
    // TODO: Implement search method to initialize iterator
    return iterator;
}

class InMemorySearchIterator : public SearchIterator {
public:
    InMemorySearchIterator(){};
    ~InMemorySearchIterator() override;

    Item& operator*() override;
    SearchIterator& operator++() override;
    bool operator!=(const SearchIterator& other) const override;
};

Item& InMemorySearchIterator::operator*() {
    // TODO: Implement operator* method
    // Return a reference to the current item in the search
}

SearchIterator& InMemorySearchIterator::operator++() {
    // TODO: Implement operator++ method
    // Move the iterator to the next item in the search and return a reference to the
    // updated iterator
}

bool InMemorySearchIterator::operator!=(const SearchIterator& other) const {
    // TODO: Implement operator!= method
    // Compare this iterator to another iterator and return true if they are not equal,
    // false otherwise
}

void InMemoryTable::output(std::ostream& out) {
    // TODO: Implement output method
}

bool InMemoryTable::import(const std::string& filename) {
    // TODO: Implement import method
    return false;
}

bool InMemoryTable::removeByParentKey(KeyType parent_key) {
    // TODO: Implement removeByParentKey method
    return false;
}

Table* InMemoryTable::searchByParentKey(KeyType parent_key) {
    // TODO: Implement searchByParentKey method
    return nullptr;
}
