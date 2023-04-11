Develop in C: 

1. An application that allows the user to interactively work with a hash table using linear probing. 
2. A library that provides functionality for working with a hash table.  

The hash table is specified by the structure:

```c
struct Table {
    KeySpace *ks;    // pointer to the keyspace 
    IndexType msize; // size of the keyspace 
}; 
```

Elements of the hash table are defined by the following structure:

```c 
struct KeySpace {
    KeyType key;      // element key 
    RelType release;  // element version number 
    Node *node;       // pointer to information 
    KeySpace *next;   // pointer to the next element 
}; 

struct Node {
    RelType release;   // element version number 
    Item *info;        // pointer to information 
    Node *next;        // pointer to the next element 
}; 
```

The following operations must be provided:

1. Inserting a new element into the table; 
2. Deleting an element from the table by key and version number; 
3. Searching for elements in the table by key; 
4. Outputting the contents of the table; 
5. Importing data from a text file; 
6. Searching for all versions of an element in the table by key; 
7. Deleting an element from the table by key.

Additional tasks:

1. Implement the search as an iterator. 
2. Perform all operations with a hash table located in external memory. 
3. Buffering file operations and caching records in external memory.

Restrictions:

The maximum size of the main keyspace area is limited to the value `msize`. 
The table can contain several elements with the same keys and different version numbers.

Data types: key - unsigned integer; value - unsigned integer.
