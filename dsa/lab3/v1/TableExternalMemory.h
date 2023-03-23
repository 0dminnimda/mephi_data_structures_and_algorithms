#include <iostream>
#include <string>
#include <vector>

typedef int KeyType;
typedef std::string InfoType;
typedef int IndexType;
typedef int PointerType;

struct Item {
    InfoType info;
    KeyType key;
};

class TableExternalMemory {
  public:
    TableExternalMemory(const std::string &fileName);
    ~TableExternalMemory();

    bool add(const Item &item);
    bool remove(KeyType key);
    std::vector<Item> search(KeyType key);
    // Iterator search(KeyType key)
    void print();

    class Iterator;

  private:
    std::string fileName;
    IndexType keySpaceIndex;
    IndexType itemIndex;
    std::vector<IndexType> keySpaceOffsets;
    std::vector<IndexType> itemOffsets;

    IndexType readIndex(std::ifstream &file);
    void writeIndex(std::ofstream &file, IndexType index);
    void readKeySpaceOffsets(std::ifstream &file);
    void writeKeySpaceOffsets(std::ofstream &file);
    void readItemOffsets(std::ifstream &file);
    void writeItemOffsets(std::ofstream &file);
    void readItem(std::ifstream &file, IndexType offset, Item &item);
    void writeItem(std::ofstream &file, IndexType offset, const Item &item);
};

class TableExternalMemory::Iterator {
  public:
    Iterator(const std::string &fileName, const std::vector<IndexType> &keySpaceOffsets, KeyType key);

    Item *next();

  private:
    const std::string &fileName;
    const std::vector<IndexType> &keySpaceOffsets;
    KeyType key;
    int index;
};
