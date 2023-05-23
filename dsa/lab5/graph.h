#include <stdbool.h>
#include <stddef.h>

typedef struct Edge {
    int attitude;
    struct Vertex *dest;
    struct Edge *next;
} Edge;

typedef struct Vertex {
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

Vertex *add_vertex(Graph *graph, const char *name);
Vertex *find_vertex(Graph *graph, const char *name);
Result add_edge(Graph *graph, Vertex *src, Vertex *dst, int attitude);
Result remove_vertex(Graph *graph, Vertex *src);
Result remove_edge(Graph *graph, Vertex *src, Vertex *dst);
Result set_vertex_name(Graph *graph, Vertex *src, const char *new_name);
Result set_edge_attitude(Graph *graph, Vertex *src, Vertex *dst, int new_attitude);
void fprint_matrix(FILE *stream, Graph *graph);
void fprint_adj_list(FILE *stream, Graph *graph);
Vertex **find_potential_friends(Graph *graph, Vertex *src);
Graph *shortest_path_bellman_ford(Graph *graph, Vertex *src, Vertex *dst);
Graph *partition_connected_components(Graph *graph);
void free_graph(Vertex *graph);
