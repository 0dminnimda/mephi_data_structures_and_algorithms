#include "graph.h"

#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main() {
    // Implement the dialog functions here to interact with the user and call the
    // appropriate functions. Make sure to validate user input and handle errors
    // accordingly.
    return 0;
}

Vertex *create_vertex(const char *name) {
    Vertex *new_vertex = (Vertex *)malloc(sizeof(Vertex));
    new_vertex->name = strdup(name);
    new_vertex->connections = NULL;
    new_vertex->next = NULL;
    return new_vertex;
}

Edge *create_edge(Vertex *dest, int attitude) {
    Edge *new_edge = (Edge *)malloc(sizeof(Edge));
    new_edge->attitude = attitude;
    new_edge->dest = dest;
    new_edge->next = NULL;
    return new_edge;
}

void add_vertex(Vertex **graph, Vertex *new_vertex) {
    new_vertex->next = *graph;
    *graph = new_vertex;
}

void add_edge(Vertex *src, Edge *new_edge) {
    new_edge->next = src->connections;
    src->connections = new_edge;
}

int remove_vertex(Vertex **graph, const char *name) {
    Vertex *prev = NULL;
    Vertex *current = *graph;
    while (current) {
        if (strcmp(current->name, name) == 0) {
            if (prev) {
                prev->next = current->next;
            } else {
                *graph = current->next;
            }
            free_graph(current);
            return 0;
        }
        prev = current;
        current = current->next;
    }
    return -1;
}

int remove_edge(Vertex *src, const char *dest_name) {
    Edge *prev = NULL;
    Edge *current = src->connections;
    while (current) {
        if (strcmp(current->dest->name, dest_name) == 0) {
            if (prev) {
                prev->next = current->next;
            } else {
                src->connections = current->next;
            }
            free(current);
            return 0;
        }
        prev = current;
        current = current->next;
    }
    return -1;
}

int change_vertex_data(Vertex *graph, const char *name, const char *new_name) {
    Vertex *vertex = find_vertex(graph, name);
    if (vertex) {
        free(vertex->name);
        vertex->name = strdup(new_name);
        return 0;
    }
    return -1;
}

int change_edge_data(Vertex *graph, const char *src_name, const char *dest_name,
                     int new_attitude) {
    Vertex *src = find_vertex(graph, src_name);
    if (src) {
        Edge *current = src->connections;
        while (current) {
            if (strcmp(current->dest->name, dest_name) == 0) {
                current->attitude = new_attitude;
                return 0;
            }
            current = current->next;
        }
    }
    return -1;
}

void print_graph(Vertex *graph) {
    Vertex *current_vertex = graph;
    while (current_vertex) {
        printf("%s: ", current_vertex->name);
        Edge *current_edge = current_vertex->connections;
        while (current_edge) {
            printf("(%s, %d) ", current_edge->dest->name, current_edge->attitude);
            current_edge = current_edge->next;
        }
        printf("\n");
        current_vertex = current_vertex->next;
    }
}

Vertex *find_vertex(Vertex *graph, const char *name) {
    Vertex *current = graph;
    while (current) {
        if (strcmp(current->name, name) == 0) { return current; }
        current = current->next;
    }
    return NULL;
}

// Implement depth_first_search, bellman_ford, and connected_components functions here.

void free_graph(Vertex *graph) {
    if (graph) {
        free_graph(graph->next);
        free(graph->name);
        Edge *current_edge = graph->connections;
        while (current_edge) {
            Edge *next_edge = current_edge->next;
            free(current_edge);
            current_edge = next_edge;
        }
        free(graph);
    }
}
