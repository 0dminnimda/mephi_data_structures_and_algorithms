#pragma once

#include <cstddef>
#include <iostream>

// Here's a possible implementation of an interactive application program for working with a vector-based table with two
// libraries for working with a table in main memory and external memory:

typedef unsigned int KeyType;
typedef unsigned int InfoType;

struct Item {
    // pointer to information
    InfoType *info;
    // optional fields, to optimize the execution of operations,
    // composition and presence of which must be justified:
    // element key
    KeyType key;
    // link to keyspace element by index
    std::size_t ind;
    // link to keyspace element by pointer
    Item *p1;
};

struct KeySpace {
    // non-null element key
    KeyType key;
    // parent element key, can be null
    KeyType par;
    // pointer to information
    Item *info;
};

class Table {
  public:
    Table(std::size_t size);
    virtual ~Table();

    virtual bool add_element(KeyType key, KeyType parent_key, InfoType *info) = 0;
    virtual void delete_element(KeyType key) = 0;
    virtual KeySpace *search_by_parent_key(KeyType parent_key) = 0;
    virtual void output_table(std::ostream &os) = 0;
    virtual void import_data(const std::string &filename) = 0;
};
