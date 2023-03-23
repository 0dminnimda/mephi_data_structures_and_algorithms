#pragma once

#include "table.h"

class TableMem : public Table {
public:
    TableMem(std::size_t size);
    virtual ~TableMem();

    virtual bool add_element(KeyType key, KeyType parent_key, InfoType *info) override;
    virtual void delete_element(KeyType key) override;
    virtual KeySpace* search_by_parent_key(KeyType parent_key) override;
    virtual void output_table(std::ostream& os) override;
    virtual void import_data(const std::string& filename) override;

private:
    KeySpace* ks;
    std::size_t num_elements;
};
