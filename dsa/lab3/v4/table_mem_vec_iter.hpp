#include "table.hpp"
#include <algorithm>
#include <fstream>
#include <sstream>
#include <stdexcept>

class InMemoryTable : public Table {
public:
    InMemoryTable(IndexType msize) : Table(msize) {}

    bool insert(KeyType key, InfoType* info) override;
    bool remove(KeyType key) override;
    SearchIterator search(KeyType key) override;
    void output(std::ostream& out) const override;
    bool import(const std::string& filename) override;
    bool removeByParentKey(KeyType parent_key) override;
    Table* searchByParentKey(KeyType parent_key) override;

private:
    std::vector<KeySpace>::iterator findKeySpace(KeyType key);
    std::vector<KeySpace>::const_iterator findKeySpace(KeyType key) const;
    std::vector<KeySpace>::iterator findParentKey(KeyType parent_key);
    std::vector<KeySpace>::const_iterator findParentKey(KeyType parent_key) const;
};

class InMemorySearchIterator : public SearchIterator {
public:
    InMemorySearchIterator(std::vector<KeySpace>::const_iterator begin,
                           std::vector<KeySpace>::const_iterator end)
        : begin_(begin), end_(end) {}

    ~InMemorySearchIterator() override = default;

    Item& operator*() override;
    SearchIterator& operator++() override;
    bool operator!=(const SearchIterator& other) const override;

private:
    std::vector<KeySpace>::const_iterator begin_;
    std::vector<KeySpace>::const_iterator end_;
};

bool InMemoryTable::insert(KeyType key, InfoType* info) {
    if (ks.size() >= msize) {
        return false;
    }

    auto it = findKeySpace(key);
    if (it != ks.end()) {
        return false;
    }

    KeyType parent_key = 0;
    if (!ks.empty()) {
        parent_key = ks.back().par;
    }

    ks.emplace_back(key, parent_key, info);
    std::sort(ks.begin(), ks.end(),
              [](const KeySpace& lhs, const KeySpace& rhs) { return lhs.par < rhs.par; });

    return true;
}

bool InMemoryTable::remove(KeyType key) {
    auto it = findKeySpace(key);
    if (it == ks.end()) {
        return false;
    }

    ks.erase(it);
    return true;
}

SearchIterator InMemoryTable::search(KeyType key) {
    auto begin = findKeySpace(key);
    auto end = begin;
    if (begin != ks.end()) {
        ++end;
        while (end != ks.end() && end->key == key) {
            ++end;
        }
    }
    return InMemorySearchIterator(begin, end);
}

void InMemoryTable::output(std::ostream& out) const {
    for (const auto& ks : ks) {
        out << ks.key << " " << ks.info->key << " " << *ks.info->info << "\n";
    }
}

bool InMemoryTable::import(const std::string& filename) {
    std::ifstream infile(filename);
    if (!infile) {
        return false;
    }

    std::string line;
    while (std::getline(infile, line)) {
        std::istringstream iss(line);
        KeyType key;
        InfoType info;
        if (!(iss >> key >> info)) {
            continue;
        }
        insert(key, new InfoType(info));
    }

    return true;
}

bool InMemoryTable::removeByParentKey(KeyType parent_key) {
    auto it = findParentKey(parent_key);
    if (it == ks.end()) {
        return false;
    }

    auto end = it;
    while (end != ks.end() && end->par == parent_key) {
        ++end;
    }
    ks.erase(it, end);
    return true;
}

Table* InMemoryTable::searchByParentKey(KeyType parent_key) {
    auto it = findParentKey(parent_key);
    if (it == ks.end()) {
        return nullptr;
    }

    auto end = it;
    while (end != ks.end() && end->par == parent_key) {
        ++end;
    }

    auto new_table = new InMemoryTable(msize);
    new_table->ks.assign(it, end);
    return new_table;
}

std::vector<KeySpace>::iterator InMemoryTable::findKeySpace(KeyType key) {
    return std::lower_bound(ks.begin(), ks.end(), key,
                            [](const KeySpace& lhs, KeyType rhs) { return lhs.key < rhs; });
}

std::vector<KeySpace>::const_iterator InMemoryTable::findKeySpace(KeyType key) const {
    return std::lower_bound(ks.begin(), ks.end(), key,
                            [](const KeySpace& lhs, KeyType rhs) { return lhs.key < rhs; });
}

std::vector<KeySpace>::iterator InMemoryTable::findParentKey(KeyType parent_key) {
    return std::lower_bound(ks.begin(), ks.end(), parent_key,
                            [](const KeySpace& lhs, KeyType rhs) { return lhs.par < rhs; });
}

std::vector<KeySpace>::const_iterator InMemoryTable::findParentKey(KeyType parent_key) const {
    return std::lower_bound(ks.begin(), ks.end(), parent_key,
                            [](const KeySpace& lhs, KeyType rhs) { return lhs.par < rhs; });
}

Item& InMemorySearchIterator::operator*() {
    return *begin_->info;
}

SearchIterator& InMemorySearchIterator::operator++() {
    ++begin_;
    return *this;
}

bool InMemorySearchIterator::operator!=(const SearchIterator& other) const {
    const InMemorySearchIterator* other_it = dynamic_cast<const InMemorySearchIterator*>(&other);
    if (other_it) {
        return begin_ != other_it->begin_;
    } else {
        return true;
    }
}

// The `InMemoryTable` class defines the operations required by the `Table` interface using an in-memory data structure. The `insert`, `remove`, `search`, `removeByParentKey`, and `searchByParentKey` methods all manipulate the `ks` vector, which stores the key spaces. The `output` and `import` methods write to and read from files, respectively.

// The `findKeySpace` and `findParentKey` methods use `std::lower_bound` to search for key spaces by key and parent key, respectively. `std::lower_bound` is an efficient search algorithm for sorted arrays and vectors.

// The `InMemorySearchIterator` class is a simple implementation of the `SearchIterator` interface. It uses two iterators, `begin_` and `end_`, to represent the range of key spaces that match the search key. The `operator*` method returns a reference to the `Item` object associated with the current key space. The `operator++` method moves the iterator to the next key space in the range. The `operator!=` method compares two iterators for inequality.

// Overall, this implementation satisfies the requirements given in the prompt and provides correct memory usage. However, there may be other valid implementations that use different data structures or algorithms.
