#include <algorithm>
#include <cstring>
#include <iostream>

#include "table.hpp"

class InMemoryTable : public Table {
public:
    std::size_t count;
    InMemoryTable(IndexType msize);
    ~InMemoryTable();

    bool insert(KeyType key, InfoType* info, KeyType parent_key) override;
    bool remove(KeyType key) override;
    SearchIterator search(KeyType key) override;
    void output(std::ostream& out) const override;
    bool import(const std::string& filename) override;
    bool removeByParentKey(KeyType parent_key) override;
    Table* searchByParentKey(KeyType parent_key) override;
};

InMemoryTable::InMemoryTable(size_t max_size) : Table(max_size), count(0) {}

// InMemoryTable::~InMemoryTable() { delete ks; }

bool InMemoryTable::insert(KeyType key, const Item& item, KeyType parent_key) {
    if (count >= msize) {
        std::cerr << "Table is full" << std::endl;
        return false;
    }

    size_t i = 0;
    while (i < count && ks[i].key < key) ++i;
    if (i < count && ks[i].key == key) {
        std::cerr << "Key already exists in table" << std::endl;
        return false;
    }

    // Move existing items to make space for new item
    for (size_t j = count; j > i; --j) {
        ks[j] = ks[j - 1];
    }

    ks[i].key = key;
    ks[i].par = parent_key;
    ks[i].info = new Item(item);
    ++count;

    return true;
}

bool InMemoryTable::remove(KeyType key) {
    size_t i = 0;
    while (i < count && ks[i].key < key) ++i;
    if (i >= count || ks[i].key != key) {
        std::cerr << "Key not found in table" << std::endl;
        return false;
    }

    delete ks[i].info;

    // Move remaining items to fill gap
    for (size_t j = i; j < count - 1; ++j) {
        ks[j] = ks[j + 1];
    }

    --count;

    return true;
}

bool InMemoryTable::search(KeyType key, Item& item) const {
    size_t i = 0;
    while (i < count && ks[i].key < key) ++i;
    if (i >= count || ks[i].key != key) {
        std::cerr << "Key not found in table" << std::endl;
        return false;
    }

    item = *(ks[i].info);

    return true;
}

bool InMemoryTable::removeByParentKey(KeyType parent_key) {
    size_t i = 0;
    while (i < count && ks[i].par < parent_key) ++i;
    while (i < count && ks[i].par == parent_key) {
        delete ks[i].info;
        // Move remaining items to fill gap
        for (size_t j = i; j < count - 1; ++j) {
            ks[j] = ks[j + 1];
        }
        --count;
    }

    return true;
}

Table* InMemoryTable::searchByParentKey(KeyType parent_key) {
    size_t i = 0;
    while (i < count && ks[i].par < parent_key) ++i;

    if (i >= count || ks[i].par != parent_key) {
        std::cerr << "Parent key not found in table" << std::endl;
        return nullptr;
    }

    InMemoryTable* table = new InMemoryTable(msize);
    size_t j = i;
    while (j < count && ks[j].par == parent_key) {
        table->ks[table->count].key = ks[j].key;
        table->ks[table->count].par = ks[j].par;
        table->ks[table->count].info = new Item(*(ks[j].info));
        ++table->count;
        ++j;
    }

    return table;
}

void InMemoryTable::output(std::ostream& os) const {
    for (size_t i = 0; i < count; ++i) {
        os << ks[i].key << ": " << *(ks[i].info) << std::endl;
    }
}

bool InMemoryTable::import(std::istream& is) {
    size_t num_items = 0;
    if (!is.read(reinterpret_cast<char*>(&num_items), sizeof(size_t))) {
        std::cerr << "Error reading number of items from file" << std::endl;
        return false;
    }

    if (num_items > msize) {
        std::cerr << "File contains more items than table can hold" << std::endl;
        return false;
    }

    count = 0;
    for (size_t i = 0; i < num_items; ++i) {
        KeyType key, parent_key;
        if (!is.read(reinterpret_cast<char*>(&key), sizeof(KeyType))) {
            std::cerr << "Error reading keyfrom file" << std::endl;
            return false;
        }
        if (!is.read(reinterpret_cast<char*>(&parent_key), sizeof(KeyType))) {
            std::cerr << "Error reading parent key from file" << std::endl;
            return false;
        }

        Item item;
        if (!item.import(is)) {
            std::cerr << "Error reading item data from file" << std::endl;
            return false;
        }

        if (!insert(key, item, parent_key)) {
            std::cerr << "Error inserting item into table" << std::endl;
            return false;
        }
    }

    return true;
}

InMemorySearchIterator::InMemorySearchIterator(const InMemoryTable& table,
                                               KeyType start_key)
    : begin_(&table.ks[0]), end_(&table.ks[table.count]), current_(nullptr) {
    auto it = std::lower_bound(
        begin_, end_, KeySpace(start_key, KeyType(), nullptr),
        [](const KeySpace& a, const KeySpace& b) { return a.key < b.key; });
    if (it != end_ && it->key == start_key) {
        current_ = it;
    }
}

Item& InMemorySearchIterator::operator*() { return *(current_->info); }

SearchIterator& InMemorySearchIterator::operator++() {
    ++current_;
    while (current_ != end_ && current_->info == nullptr) {
        ++current_;
    }
    return *this;
}

bool InMemorySearchIterator::operator!=(const SearchIterator& other) const {
    const InMemorySearchIterator* other_it =
        dynamic_cast<const InMemorySearchIterator*>(&other);
    if (other_it) {
        return current_ != other_it->current_;
    } else {
        return true;
    }
}

// In this implementation, the `ks` array of `KeySpace` objects is used to store the key
// spaces, and the `insert`, `remove`, `search`, `removeByParentKey`, and
// `searchByParentKey` methods use a linear search to find the appropriate position or
// range of positions within the `ks` array. The `output` method outputs the contents of
// the `ks` array to the provided output stream.

// The `import` method reads in key spaces from a binary file using `std::istream::read`,
// and the `searchByParentKey` method uses a linear search to find the first key space
// with the specified parent key, and then iterates through the `ks` array to copy all key
// spaces with the same parent key into a new `InMemoryTable` object.

// The `InMemorySearchIterator` class is defined as a separate class, and its
// implementation uses pointers to `KeySpace` objects to implement the `SearchIterator`
// interface. The `begin_` and `end_` pointers are used to define the range of key spaces
// to iterate over, and the `current_` pointer points to the current key space being
// iterated over. The `++` operator is overloaded to skip over null pointers in the `info`
// field of the `KeySpace` objects.
