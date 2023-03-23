#include "table_mem.h"

#include <iostream>
#include <fstream>
#include <sstream>

TableMem::TableMem(std::size_t size) : Table(size), ks(new KeySpace[size]), num_elements(0) {}

TableMem::~TableMem() {
    for (std::size_t i = 0; i < num_elements; ++i) {
        delete ks[i].info;
    }
    delete[] ks;
}

bool TableMem::add_element(KeyType key, KeyType parent_key, InfoType *info) {
    bool found = false;
    for (std::size_t i = 0; i < num_elements; ++i) {
        if (ks[i].key == key) {
            std::cerr << "Error: element with key " << key << " already exists" << std::endl;
            found = true;
            break;
        }
    }
    if (!found) {
        ks[num_elements].key = key;
        ks[num_elements].par = parent_key;
        ks[num_elements].info = new Item{ info, key, num_elements, nullptr };
        ++num_elements;
    }
    return found;
}

void TableMem::delete_element(KeyType key) {
    bool found = false;
    for (std::size_t i = 0; i < num_elements; ++i) {
        if (ks[i].key == key) {
            found = true;
            delete ks[i].info;
            for (std::size_t j = i; j < num_elements - 1; ++j) {
                ks[j] = ks[j + 1];
                ks[j].info->ind = j;
            }
            --num_elements;
            break;
        }
    }
    if (!found) {
        std::cerr << "Error: element with key " << key << " not found" << std::endl;
    }
}

KeySpace* TableMem::search_by_parent_key(KeyType parent_key) {
    KeySpace* result = nullptr;
    for (std::size_t i = 0; i < num_elements; ++i) {
        if (ks[i].par == parent_key) {
            if (result == nullptr) {
                result = &ks[i];
            }
            else {
                Item* current = result->info;
                while (current->p1 != nullptr) {
                    current = current->p1;
                }
                current->p1 = ks[i].info;
            }
        }
    }
    return result;
}

void TableMem::output_table(std::ostream& os) {
    for (std::size_t i = 0; i < num_elements; ++i) {
        os << ks[i].key << "," << ks[i].par << "," << *(ks[i].info->info) << std::endl;
    }
}

void TableMem::import_data(const std::string& filename) {
    std::ifstream ifs(filename);
    if (!ifs) {
        std::cerr << "Error: cannot open file " << filename << std::endl;
        return;
    }
    std::string line;
    while (std::getline(ifs, line)) {
        std::istringstream iss(line);
        KeyType key, parent_key;
        InfoType info;
        char comma;
        if (iss >> key >> comma >> parent_key >> comma >> info) {
            add_element(key, parent_key, new InfoType(info));
        }
        else {
            std::cerr << "Error: invalid input format in line " << line << std::endl;
        }
    }
}
