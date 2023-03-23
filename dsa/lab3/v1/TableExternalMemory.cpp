#include "TableExternalMemory.h"
#include <fstream>
#include <iostream>
#include <sstream>

TableExternalMemory::TableExternalMemory(const std::string &fileName)
    : fileName(fileName), keySpaceIndex(0), itemIndex(0) {
    std::ifstream file(fileName, std::ios::binary);
    if (!file) {
        keySpaceOffsets.push_back(0);
        itemOffsets.push_back(0);
        writeKeySpaceOffsets(file);
        writeItemOffsets(file);
    } else {
        file.seekg(0, std::ios::end);
        if (file.tellg() > 0) {
            file.seekg(0, std::ios::beg);
            keySpaceIndex = readIndex(file);
            itemIndex = readIndex(file);
            readKeySpaceOffsets(file);
            readItemOffsets(file);
        } else {
            keySpaceOffsets.push_back(0);
            itemOffsets.push_back(0);
            writeKeySpaceOffsets(file);
            writeItemOffsets(file);
        }
    }
}

TableExternalMemory::~TableExternalMemory() {
    std::ofstream file(fileName, std::ios::binary);
    writeIndex(file, keySpaceIndex);
    writeIndex(file, itemIndex);
    writeKeySpaceOffsets(file);
    writeItemOffsets(file);
}

bool TableExternalMemory::add(const Item &item) {
    for (const auto &offset : keySpaceOffsets) {
        Item temp;
        readItem(std::ifstream(fileName, std::ios::binary), offset, temp);
        if (temp.key == item.key) {
            return false;
        }
    }
    Item newItem = item;
    newItem.keySpaceOffset = keySpaceOffsets[keySpaceIndex];
    newItem.itemOffset = itemOffsets[itemIndex];
    std::ofstream file(fileName, std::ios::binary | std::ios::app);
    writeItem(file, newItem.itemOffset, newItem);
    itemOffsets.push_back(file.tellp());
    ++itemIndex;
    keySpaceOffsets[keySpaceIndex] = file.tellp();
    ++keySpaceIndex;
    writeKeySpaceOffsets(file);
    return true;
}

bool TableExternalMemory::remove(KeyType key) {
    for (const auto &offset : keySpaceOffsets) {
        Item temp;
        readItem(std::ifstream(fileName, std::ios::binary), offset, temp);
        if (temp.key == key) {
            std::ofstream file(fileName, std::ios::binary);
            temp.info = "";
            writeItem(file, offset, temp);
            return true;
        }
    }
    return false;
}

std::vector<Item> TableExternalMemory::search(KeyType key) {
    std::vector<Item> result;
    for (const auto &offset : keySpaceOffsets) {
        Item temp;
        readItem(std::ifstream(fileName, std::ios::binary), offset, temp);
        if (temp.key == key) {
            result.push_back(temp);
        }
    }
    return result;
}

// TableExternalMemory::Iterator search(KeyType key) { return TableExternalMemory::Iterator(fileName, keySpaceOffsets,
// key); }

void TableExternalMemory::print() {
    for (const auto &offset : keySpaceOffsets) {
        Item temp;
        readItem(std::ifstream(fileName, std::ios::binary), offset, temp);
        if (temp.info != "") {
            std::cout << "key: " << temp.key << ", info: " << temp.info << std::endl;
        }
    }
}

IndexType TableExternalMemory::readIndex(std::ifstream &file) {
    IndexType index;
    file.read(reinterpret_cast<char *>(&index), sizeof(index));
    return index;
}

void TableExternalMemory::writeIndex(std::ofstream &file, IndexType index) {
    file.write(reinterpret_cast<char *>(&index), sizeof(index));
}

void TableExternalMemory::readKeySpaceOffsets(std::ifstream &file) {
    IndexType count;
    file.read(reinterpret_cast<char *>(&count), sizeof(count));
    keySpaceOffsets.resize(count);
    for (auto &offset : keySpaceOffsets) {
        file.read(reinterpret_cast<char *>(&offset), sizeof(offset));
    }
}

void TableExternalMemory::writeKeySpaceOffsets(std::ofstream &file) {
    IndexType count = keySpaceOffsets.size();
    file.write(reinterpret_cast<char *>(&count), sizeof(count));
    for (const auto &offset : keySpaceOffsets) {
        file.write(reinterpret_cast<const char *>(&offset), sizeof(offset));
    }
}

void TableExternalMemory::readItemOffsets(std::ifstream &file) {
    IndexType count;
    file.read(reinterpret_cast<char *>(&count), sizeof(count));
    itemOffsets.resize(count);
    for (auto &offset : itemOffsets) {
        file.read(reinterpret_cast<char *>(&offset), sizeof(offset));
    }
}

void TableExternalMemory::writeItemOffsets(std::ofstream &file) {
    IndexType count = itemOffsets.size();
    file.write(reinterpret_cast<char *>(&count), sizeof(count));
    for (const auto &offset : itemOffsets) {
        file.write(reinterpret_cast<const char *>(&offset), sizeof(offset));
    }
}

void TableExternalMemory::readItem(std::ifstream &file, IndexType offset, Item &item) {
    file.seekg(offset);
    file.read(reinterpret_cast<char *>(&item), sizeof(item));
}

void TableExternalMemory::writeItem(std::ofstream &file, IndexType offset, const Item &item) {
    file.seekp(offset);
    file.write(reinterpret_cast<const char *>(&item), sizeof(item));
}

TableExternalMemory::Iterator::Iterator(const std::string &fileName, const std::vector<IndexType> &keySpaceOffsets,
                                        KeyType key)
    : fileName(fileName), keySpaceOffsets(keySpaceOffsets), key(key), index(-1) {}

Item *TableExternalMemory::Iterator::next() {
    while (++index < keySpaceOffsets.size()) {
        Item temp;
        readItem(std::ifstream(fileName, std::ios::binary), keySpaceOffsets[index], temp);
        if (temp.key == key) {
            return new Item(temp);
        }
    }
    return nullptr;
}
