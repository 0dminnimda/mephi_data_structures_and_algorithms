Complete the task (give me the unimplemented functions):

Write a program in C language that implements working with a directed graph. Use those:

```c
typedef struct Edge {
    int attitude;
    struct Vertex *dest;
    struct Edge *next;
} Edge;

typedef struct Vertex {
    int id;
    char *name;
    Edge *connections;
    struct Vertex *next;
    struct Vertex *prev;
} Vertex;

typedef struct Graph {
    Vertex *vertices;
    size_t size;
} Graph;

typedef enum Result { SUCCESS = true, FAILURE = false } Result;

// ALREADY IMPLEMENTED ELSEWHERE
void free_graph(Graph *graph);
Vertex *add_vertex(Graph *graph, const char *name);
Vertex *find_vertex(Graph *graph, const char *name);
Result add_edge(Graph *graph, Vertex *src, Vertex *dst, int attitude);
Result remove_vertex(Graph *graph, Vertex *src);
Result remove_edge(Graph *graph, Vertex *src, Vertex *dst);
Result set_vertex_name(Graph *graph, Vertex *src, const char *new_name);
Result set_edge_attitude(Graph *graph, Vertex *src, Vertex *dst, int new_attitude);
void fprint_matrix(FILE *stream, Graph *graph);
void fprint_adj_list(FILE *stream, Graph *graph);
```

The program should support the following operations:

1. Traverse the graph in depth to determine the list of people that the specified person could potentially be friends with - in the chain of pairs of acquaintances, there is no pair of people who have a negative relationship with each other.

2. Find the shortest path between two given vertices of the graph using the Bellman-Ford algorithm.

3. Partition the group of people under consideration into subgroups, inside which there is a chain of acquaintances with an exclusively positive relationship between the pair of people who make up this chain (partition into connected or strongly connected components).

The program should also support common operations on the graph:

1. Adding a new vertex.

2. Adding a new edge between two given vertices.

3. Removing a given vertex.

4. Removing a given edge.

5. Changing data associated with a given vertex.

6. Changing data associated with a given edge.

7. Output the graph as a matrix or adjacency lists.

The graph should be directed and represent a circle of acquaintances. The vertices of the graph should store a unique person's name (string), and the edges of the graph should store the level of relationship with the person (an integer in the range from -10 to 10).

Logically complete parts of the algorithm for solving the problem should be implemented as separate functions with parameters. The use of global variables is not allowed.

Functions for working with the graph should not be dialog, i.e. they should accept all necessary data as parameters and return the result of the work in the form of corresponding data structures and error codes (exception: functions for outputting the graph).

Dialog functions should use the described functions above.

The program should perform validation of entered data and, in case of errors, issue corresponding messages, after which it should continue working.

In case of errors during operations on the graph, the program should output corresponding messages, after which it should continue working.

The program should work correctly with memory. To check this, appropriate software tools should be used, such as valgrind, sanitizers, IDE built-in tools, etc.
