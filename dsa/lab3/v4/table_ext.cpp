/*
Write program in C++:
Write program in C++, but leave the actual implementation blank (aka make a skeleton):

- Library for vector-based table
- Console application to work with library table interactively

Library implementations:

1. In-memory
2. External memory (binary file)

Vector-based table:

`table.h`:
```c
using KeyType = unsigned int;
using InfoType = unsigned int;
using IndexType = size_t;

class Item {
    InfoType *info; // actual data
    KeyType key; // backreference to KeySpace
};

class KeySpace {
    KeyType key;
    KeyType par; // nullable
    unique_ptr<Item> info;
};

class Table {
    vector<KeySpace> ks;
    IndexType msize; // max key space size
};
```

No dup keys/must have parent key or null.
Ordered by parent key. Parent key may duplicate.

Operations:

- Insert element with unique key
- Delete element by key
- Search by key (returns an iterator of matches)
- Output to standard stream
- Import from text file
- Remove by key if no dup parent keys
- Search by parent key value, return new table of matches

Use buffering and caching for external memory

Requirements:

- No globals
- If input incorrect, show error, then continue
- If error in table op, show it, then continue
- Modified struct for external memory (add length/offset)
- Use `fread`/`fwrite` for files (with length)
- Correct memory usage
*/
