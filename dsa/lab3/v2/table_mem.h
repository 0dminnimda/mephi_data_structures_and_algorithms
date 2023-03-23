#ifndef TABLE_MEM_H
#define TABLE_MEM_H

#include "table_base.h"
#include <vector>

class TableMem : public Table {
  public:
    TableMem();
    ~TableMem();
    bool add_element(unsigned int key, unsigned int parent_key, unsigned int value) override;
    bool delete_element(unsigned int key) override;
    std::vector<Element *> search_by_parent_key(unsigned int parent_key) override;
    void output() override;
    bool import_data(std::string filename) override;
    bool is_file() override;

  private:
    std::vector<Element *> elements;
};

TableMem::TableMem() {
    // Constructor
}

TableMem::~TableMem() {
    // Destructor
    for (Element *e : elements) {
        delete e;
    }
}

bool TableMem::add_element(unsigned int key, unsigned int parent_key, unsigned int value) {
    // Check if key already exists
    for (Element *e : elements) {
        if (e->key == key) {
            return false;
        }
    }

    // Add new element
    Element *e = new Element(key, parent_key, value);
    elements.push_back(e);
    return true;
}

bool TableMem::delete_element(unsigned int key) {
    // Find the element with the key
    for (auto it = elements.begin(); it != elements.end(); it++) {
        if ((*it)->key == key) {
            // Delete the element and erase from vector
            delete *it;
            elements.erase(it);
            return true;
        }
    }

    // Key not found
    return false;
}

std::vector<Element *> TableMem::search_by_parent_key(unsigned int parent_key) {
    std::vector<Element *> result;

    // Find all elements with the parent key
    for (Element *e : elements) {
        if (e->parent_key == parent_key) {
            result.push_back(e);
        }
    }

    return result;
}

void TableMem::output() {
    // Output all elements
    for (Element *e : elements) {
        std::cout << "Key: " << e->key << ", Parent key: " << e->parent_key << ", Value: " << e->value << std::endl;
    }
}

bool TableMem::import_data(std::string filename) {
    // TODO: Implement import from text file
    return true;
}

bool TableMem::is_file() { return false; }

#endif // TABLEMEM_H
