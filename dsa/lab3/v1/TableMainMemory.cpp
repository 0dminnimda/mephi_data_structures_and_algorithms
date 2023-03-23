#include "TableMainMemory.h"
#include <iostream>

TableMainMemory::TableMainMemory() {}

TableMainMemory::~TableMainMemory() {}

bool TableMainMemory::add(const Item &item) {
    for (const auto &i : items) {
        if (i.key == item.key) {
            return false;
        }
    }
    items.push_back(item);
    return true;
}

bool TableMainMemory::remove(KeyType key) {
    for (auto it = items.begin(); it != items.end(); ++it) {
        if (it->key == key) {
            items.erase(it);
            return true;
        }
    }
    return false;
}

std::vector<Item> TableMainMemory::search(KeyType key) {
    std::vector<Item> result;
    for (const auto &i : items) {
        if (i.key == key) {
            result.push_back(i);
        }
    }
    return result;
}

// TableMainMemory::Iterator search(KeyType key) { return TableMainMemory::Iterator(table, key); }

void TableMainMemory::print() {
    for (const auto &i : items) {
        std::cout << "key: " << i.key << ", info: " << i.info << std::endl;
    }
}

TableMainMemory::Iterator::Iterator(std::vector<Item> &items, KeyType key) : items(items), key(key), index(-1) {}

Item *TableMainMemory::Iterator::next() {
    for (++index; index < items.size(); ++index) {
        if (items[index].key == key) {
            return &items[index];
        }
    }
    return nullptr;
}
