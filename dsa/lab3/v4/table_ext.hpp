#include "table.hpp"

class ExternalMemoryTable : public Table {
public:
    ExternalMemoryTable(IndexType msize) : Table(msize) {}

    bool insert(KeyType key, InfoType* info) override;
    bool remove(KeyType key) override;
    SearchIterator search(KeyType key) override;
    void output(std::ostream& out) const override;
    bool import(const std::string& filename) override;
    bool removeByParentKey(KeyType parent_key) override;
    Table* searchByParentKey(KeyType parent_key) override;
};
