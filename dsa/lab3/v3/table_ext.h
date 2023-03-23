#pragma once

#include "table.h"
#include <cstdio>

class TableExt : public Table {
public:
    TableExt(std::size_t size, const std::string& filename);
    virtual ~TableExt();

    virtual bool add_element(KeyType key, KeyType parent_key, InfoType *info) override;
    virtual void delete_element(KeyType key) override;
    virtual KeySpace* search_by_parent_key(KeyType parent_key) override;
    virtual void output_table(std::ostream& os) override;
    virtual void import_data(const std::string& filename) override;

private:
    FILE* file;
    std::size_t num_elements;
};