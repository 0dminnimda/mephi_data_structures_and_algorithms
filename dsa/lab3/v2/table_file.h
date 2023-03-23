#ifndef TABLEFILE_H
#define TABLEFILE_H

#include <fstream>
#include <iostream>
#include <string>
#include <vector>

class Element {
  public:
    unsigned int key;
    unsigned int parent_key;
    std::string value;

    Element(unsigned int k, unsigned int pk, std::string v) : key(k), parent_key(pk), value(v) {}
};

class TableFile {
  private:
    std::string filename;
    std::fstream file;
    unsigned int next_key;
    unsigned int root_key;

  public:
    TableFile(std::string fname) : filename(fname), next_key(1), root_key(0) {
        file.open(filename, std::ios::in | std::ios::out | std::ios::binary | std::ios::trunc);
        if (!file.is_open()) {
            std::cerr << "Error: Could not open file " << filename << std::endl;
        }
    }

    ~TableFile() { file.close(); }

    bool insert_element(unsigned int parent_key, std::string value);
    bool delete_element(unsigned int key);
    std::vector<Element *> search_by_parent_key(unsigned int parent_key);
    void output();
    bool import_data(std::string filename);
    bool is_file() { return true; }

  private:
    void write_element(Element *e);
    Element *read_element(unsigned int key);
    void delete_subtree(unsigned int key);
};

void TableFile::write_element(Element *e) {
    // Write the element to the end of the file
    file.seekp(0, std::ios::end);
    file.write(reinterpret_cast<char *>(&e->key), sizeof(e->key));
    file.write(reinterpret_cast<char *>(&e->parent_key), sizeof(e->parent_key));
    size_t len = e->value.length();
    file.write(reinterpret_cast<char *>(&len), sizeof(len));
    file.write(e->value.c_str(), len);
}

Element *TableFile::read_element(unsigned int key) {
    // Find the element with the key
    Element *e = nullptr;
    file.seekg(0, std::ios::end);
    size_t size = file.tellg();
    size_t pos = sizeof(unsigned int) * 2;
    while (pos < size) {
        file.seekg(pos, std::ios::beg);
        unsigned int k, pk;
        file.read(reinterpret_cast<char *>(&k), sizeof(k));
        file.read(reinterpret_cast<char *>(&pk), sizeof(pk));
        if (k == key) {
            // Found the element, read the value
            size_t len;
            file.read(reinterpret_cast<char *>(&len), sizeof(len));
            char *buf = new char[len + 1];
            file.read(buf, len);
            buf[len] = '\0';
            std::string value(buf);
            e = new Element(k, pk, value);
            delete[] buf;
            break;
        }
        pos += sizeof(unsigned int) * 2;
        size_t len;
        file.read(reinterpret_cast<char *>(&len), sizeof(len));
        pos += sizeof(len) + len;
    }
    return e;
}

bool TableFile::insert_element(unsigned int parent_key, std::string value) {
    // Create new element with next key
    Element *e = new Element(next_key++, parent_key, value);

    // Write to file
    write_element(e);

    // Update root key if this is the first element
    if (root_key == 0) {
        root_key = e->key;
    }

    delete e;
    return true;
}

bool TableFile::delete_element(unsigned int key) {
    // Find the element with the key
    Element *e = read_element(key);
    if (e == nullptr) {
        // Key not found
        return false;
    }

    // Delete the element and all its descendants
    delete_subtree(e->key);

    delete e;
    return true;
}

void TableFile::delete_subtree(unsigned int key) {
    // Find all elements with the parent key
    std::vector<unsigned int> to_delete;
    to_delete.push_back(key);
    size_t i = 0;
    while (i < to_delete.size()) {
        unsigned int current_key = to_delete[i++];
        for (Element *e : search_by_parent_key(current_key)) {
            to_delete.push_back(e->key);
        }
    }

    // Delete all elements in reverse order
    for (i = to_delete.size() - 1; i > 0; i--) {
        unsigned int current_key = to_delete[i];
        Element *e = read_element(current_key);
        if (e != nullptr) {
            delete e;
            // Write zeros to mark element as deleted
            // file.seek // Write zeros to mark element as deleted
            unsigned int zero = 0;
            size_t pos =
                (i - 1) * (sizeof(unsigned int) * 2 + sizeof(size_t) + e->value.length()) + sizeof(unsigned int);
            file.seekp(pos, std::ios::beg);
            file.write(reinterpret_cast<char *>(&zero), sizeof(zero));
            file.write(reinterpret_cast<char *>(&zero), sizeof(zero));
            size_t len = 0;
            file.write(reinterpret_cast<char *>(&len), sizeof(len));
        }
    }

    std::vector<Element *> TableFile::search_by_parent_key(unsigned int parent_key) {
        std::vector<Element *> result;

        // Iterate over all elements
        file.seekg(0, std::ios::end);
        size_t size = file.tellg();
        size_t pos = sizeof(unsigned int) * 2;
        while (pos < size) {
            file.seekg(pos, std::ios::beg);
            unsigned int k, pk;
            file.read(reinterpret_cast<char *>(&k), sizeof(k));
            file.read(reinterpret_cast<char *>(&pk), sizeof(pk));
            if (pk == parent_key) {
                // Found an element with the [parent
                // key](poe://www.poe.com/_api/key_phrase?phrase=parent%20key&prompt=Tell%20me%20more%20about%20parent%20key.),
                // read the value
                size_t len;
                file.read(reinterpret_cast<char *>(&len), sizeof(len));
                char *buf = new char[len + 1];
                file.read(buf, len);
                buf[len] = '\0';
                std::string value(buf);
                Element *e = new Element(k, pk, value);
                result.push_back(e);
                delete[] buf;
            }
            pos += sizeof(unsigned int) * 2;
            size_t len;
            file.read(reinterpret_cast<char *>(&len), sizeof(len));
            pos += sizeof(len) + len;
        }

        return result;
    }

    void TableFile::output() {
        // Output all elements
        file.seekg(0, std::ios::end);
        size_t size = file.tellg();
        size_t pos = 0;
        while (pos < size) {
            file.seekg(pos, std::ios::beg);
            unsigned int k, pk;
            file.read(reinterpret_cast<char *>(&k), sizeof(k));
            file.read(reinterpret_cast<char *>(&pk), sizeof(pk));
            size_t len;
            file.read(reinterpret_cast<char *>(&len), sizeof(len));
            char *buf = new char[len + 1];
            file.read(buf, len);
            buf[len] = '\0';
            std::string value(buf);
            std::cout << "Key: " << k << ", Parent key: " << pk << ", Value: " << value << std::endl;
            delete[] buf;
            pos += sizeof(unsigned int) * 2 + sizeof(size_t) + len;
        }
    }

    bool TableFile::import_data(std::string filename) {
        // TODO: Implement import from text file
        return true;
    }

#endif // TABLEFILE_H
