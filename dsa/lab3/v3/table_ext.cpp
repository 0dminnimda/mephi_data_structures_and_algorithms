#include "table_ext.h"

#include <iostream>
#include <fstream>
#include <sstream>

TableExt::TableExt(std::size_t size, const std::string& filename) : Table(size), num_elements(0) {
    file = fopen(filename.c_str(), "wb+");
    if (!file) {
        std::cerr << "Error: cannot open file " << filename << std::endl;
    }
}

TableExt::~TableExt() {
    fclose(file);
}

void TableExt::add_element(KeyType key, KeyType parent_key, InfoType *info) {
    bool found = false;
    for (std::size_t i = 0; i < num_elements; ++i) {
        KeySpace ks;
        if (fseek(file, i * sizeof(KeySpace), SEEK_SET) != 0) {
            std::cerr << "Error: cannot seek to element " << i << std::endl;
            return;
        }
        if (fread(&ks, sizeof(KeySpace), 1, file) != 1) {
            std::cerr << "Error: cannot read element " << i << std::endl;
            return;
        }
        if (ks.key == key) {
            std::cerr << "Error: element with key " << key << " already exists" << std::endl;
            found = true;
            break;
        }
    }
    if (!found) {
        KeySpace ks{ key, parent_key, nullptr };
        if (fseek(file, num_elements * sizeof(KeySpace), SEEK_SET) != 0) {
            std::cerr << "Error: cannot seek to element " << num_elements << std::endl;
            return;
        }
        if (fwrite(&ks, sizeof(KeySpace), 1, file) != 1) {
            std::cerr << "Error: cannot write element " << num_elements << std::endl;
            return;
        }
        if (fseek(file, 0, SEEK_END) != 0) {
            std::cerr << "Error: cannot seek to end of file" << std::endl;
            return;
        }
        std::size_t info_offset = ftell(file);
        if (fwrite(info, sizeof(InfoType), 1, file) != 1) {
            std::cerr << "Error: cannot write info" << std::endl;
            return;
        }
        if (fseek(file, num_elements * sizeof(KeySpace), SEEK_SET) != 0) {
            std::cerr << "Error: cannot seek to element " << num_elements << std::endl;
            return;
        }
        ks.info = new Item{ new InfoType(*info), key, num_elements, nullptr };
        ks.info->p1 = nullptr;
        ks.info->info = new InfoType;
        ks.info->ind = num_elements;
        ks.info->p1 = nullptr;
        if (fwrite(&ks, sizeof(KeySpace), 1, file) != 1) {
            std::cerr << "Error: cannot write element " << num_elements << std::endl;
            return;
        }
        ++num_elements;
    }
}

void TableExt::delete_element(KeyType key) {
    bool found = false;
    for (std::size_t i = 0; i < num_elements; ++i) {
        KeySpace ks;
        if (fseek(file, i * sizeof(KeySpace), SEEK_SET) != 0) {
            std::cerr << "Error: cannot seek to element i" << std::endl;
            return;
        }
        if (fread(&ks, sizeof(KeySpace), 1, file) != 1) {
            std::cerr << "Error: cannot read element " << i << std::endl;
            return;
        }
        if (ks.key == key) {
            if (ks.info) {
                if (fseek(file, ks.info->offset, SEEK_SET) != 0) {
                    std::cerr << "Error: cannot seek to info offset " << ks.info->offset << std::endl;
                    return;
                }
                if (fwrite(&ks.info->next, sizeof(std::size_t), 1, file) != 1) {
                    std::cerr << "Error: cannot write info offset " << ks.info->offset << std::endl;
                    return;
                }
            }
            ks.key = 0;
            ks.parent_key = 0;
            delete ks.info;
            ks.info = nullptr;
            if (fseek(file, i * sizeof(KeySpace), SEEK_SET) != 0) {
                std::cerr << "Error: cannot seek to element " << i << std::endl;
                return;
            }
            if (fwrite(&ks, sizeof(KeySpace), 1, file) != 1) {
                std::cerr << "Error: cannot write element " << i << std::endl;
                return;
            }
            found = true;
            break;
        }
    }
    if (!found) {
        std::cerr << "Error: element with key " << key << " not found" << std::endl;
    }
}

KeySpace* TableExt::search_by_parent_key(KeyType parent_key) {
    KeySpace* result = nullptr;
    for (std::size_t i = 0; i < num_elements; ++i) {
        KeySpace ks;
        if (fseek(file, i * sizeof(KeySpace), SEEK_SET) != 0) {
            std::cerr << "Error: cannot seek to element " << i << std::endl;
            return nullptr;
        }
        if (fread(&ks, sizeof(KeySpace), 1, file) != 1) {
            std::cerr << "Error: cannot read element " << i << std::endl;
            return nullptr;
        }
        if (ks.parent_key == parent_key) {
            result = new KeySpace{ ks.key, ks.parent_key, nullptr };
            if (ks.info) {
                result->info = new Item{ new InfoType(*ks.info->info), ks.info->key, ks.info->ind, nullptr };
                Item* curr_item = result->info;
                std::size_t next_offset = ks.info->next;
                while (next_offset != 0) {
                    if (fseek(file, next_offset, SEEK_SET) != 0) {
                        std::cerr << "Error: cannot seek to info offset " << next_offset << std::endl;
                        return nullptr;
                    }
                    Item* next_item = new Item;
                    if (fread(next_item, sizeof(Item), 1, file) != 1) {
                        std::cerr << "Error: cannot read item at offset " << next_offset << std::endl;
                        return nullptr;
                    }
                    next_item->info = new InfoType;
                    if (fread(next_item->info, sizeof(InfoType), 1, file) != 1) {
                        std::cerr << "Error: cannot read info at offset " << next_offset + sizeof(Item) << std::endl;
                        return nullptr;
                    }
                    curr_item->p1 = next_item;
                    curr_item = next_item;
                    next_offset = next_item->next;
                }
            }
            break;
        }
    }
    return result;
}

void TableExt::output_table(std::ostream& os) {
    os << "TableExt:" << std::endl;
    for (std::size_t i = 0; i < num_elements; ++i) {
        KeySpace ks;
        if (fseek(file, i * sizeof(KeySpace), SEEK_SET) != 0) {
            std::cerr << "Error: cannot seek to element " << i << std::endl;
            continue;
        }
        if (fread(&ks, sizeof(KeySpace), 1, file) != 1) {
            std::cerr << "Error: cannot read element " << i << std::endl;
            continue;
        }
        os << "  [" << i << "]: key=" << ks.key << ", parent_key=" << ks.parent_key << ", info=[";
        if (ks.info) {
            os << "(" << ks.info->key << "," << ks.info->ind << ")";
            std::size_t next_offset = ks.info->next;
            while (next_offset != 0) {
                if (fseek(file, next_offset, SEEK_SET)!= 0) {
                    std::cerr << "Error: cannot seek to info offset " << next_offset << std::endl;
                    break;
                }
                Item item;
                if (fread(&item, sizeof(Item), 1, file) != 1) {
                    std::cerr << "Error: cannot read item at offset " << next_offset << std::endl;
                    break;
                }
                os << ", (" << item.key << "," << item.ind << ")";
                next_offset = item.next;
            }
        }
        os << "]" << std::endl;
    }
}
