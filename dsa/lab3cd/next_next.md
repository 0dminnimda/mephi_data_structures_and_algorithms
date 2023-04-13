Develop in C: 

1. An application that allows the user to interactively work with a hash table using chained hashing. Each KeySpace corresponds to a linked list of versions - Nodes.
2. A library that provides functionality for working with a hash table.  

The hash table is specified by the structure:

```c
struct Table {
    KeySpace *ks;      // pointer to the keyspace 
    IndexType msize;   // size of the keyspace 
}; 
```

Elements of the hash table are defined by the following structure:

```c 
struct KeySpace {
    KeyType key;      // element key 
    Node *node;       // pointer to Node linked list
    KeySpace *next;   // pointer to the next element 
}; 

struct Node {
    RelType release;   // element version number 
    Item *info;        // pointer to Item 
    Node *next;        // pointer to the next element 
}; 

struct Item {
    InfoType *info;   // pointer to information
    KeySpace *p1;     // reference back to KeySpace
};
```

The following operations must be provided:

1. Inserting a new element into the table, release = previous + 1; 
2. Deleting an element from the table by key and version number; 
3. Searching for elements in the table by key; 
4. Outputting the contents of the table; 
5. Importing data from a text file; 
6. Searching for all versions of an element in the table by key; 
7. Deleting an element from the table by key.

Implement the search as an iterator (use single function with static local variable).

The maximum size of the main keyspace area is limited to the value `msize`. 
The table can contain several elements with the same keys and different version numbers.

Data types: key - unsigned integer; value - unsigned integer.
