#include "graph.h"

#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void free_graph(Graph *graph) {
    Vertex *current_vertex = graph->vertices;
    while (current_vertex != NULL) {
        Edge *current_edge = current_vertex->connections;
        while (current_edge != NULL) {
            Edge *next_edge = current_edge->next;
            free(current_edge);
            current_edge = next_edge;
        }
        Vertex *next_vertex = current_vertex->next;
        free(current_vertex->name);
        free(current_vertex);
        current_vertex = next_vertex;
    }
    free(graph);
}

Graph *copy_graph(Graph *src) {
    if (src == NULL) {
        return NULL;
    }

    Graph *dest = malloc(sizeof(Graph));
    dest->size = src->size;
    dest->vertices = NULL;
    Vertex *src_vertex = src->vertices;
    Vertex *prev_dest_vertex = NULL;

    while (src_vertex != NULL) {
        Vertex *dest_vertex = malloc(sizeof(Vertex));
        dest_vertex->id = src_vertex->id;
        dest_vertex->name = strdup(src_vertex->name);
        dest_vertex->connections = NULL;
        dest_vertex->next = NULL;
        dest_vertex->prev = prev_dest_vertex;

        if (prev_dest_vertex != NULL) {
            prev_dest_vertex->next = dest_vertex;
        } else {
            dest->vertices = dest_vertex;
        }

        Edge *src_edge = src_vertex->connections;
        Edge *prev_dest_edge = NULL;

        while (src_edge != NULL) {
            Edge *dest_edge = malloc(sizeof(Edge));
            dest_edge->attitude = src_edge->attitude;
            dest_edge->dest = NULL; // This will be filled later
            dest_edge->next = NULL;

            if (prev_dest_edge != NULL) {
                prev_dest_edge->next = dest_edge;
            } else {
                dest_vertex->connections = dest_edge;
            }

            prev_dest_edge = dest_edge;
            src_edge = src_edge->next;
        }

        prev_dest_vertex = dest_vertex;
        src_vertex = src_vertex->next;
    }

    // Now, set the dest pointers for the edges
    Vertex *dest_vertex = dest->vertices;
    src_vertex = src->vertices;

    while (dest_vertex != NULL) {
        Edge *dest_edge = dest_vertex->connections;
        Edge *src_edge = src_vertex->connections;

        while (dest_edge != NULL) {
            dest_edge->dest = find_vertex(dest, src_edge->dest->name);
            dest_edge = dest_edge->next;
            src_edge = src_edge->next;
        }

        dest_vertex = dest_vertex->next;
        src_vertex = src_vertex->next;
    }

    return dest;
}

Vertex *add_vertex(Graph *graph, const char *name) {
    if (find_vertex(graph, name) != NULL) { return NULL; }

    Vertex *new_vertex = (Vertex *)calloc(1, sizeof(Vertex));
    new_vertex->id = graph->size;
    new_vertex->name = strdup(name);
    new_vertex->connections = NULL;
    new_vertex->next = graph->vertices;
    new_vertex->prev = NULL;

    if (graph->vertices != NULL) { graph->vertices->prev = new_vertex; }

    graph->vertices = new_vertex;
    graph->size++;

    return new_vertex;
}

Vertex *find_vertex(Graph *graph, const char *name) {
    Vertex *current = graph->vertices;
    while (current != NULL) {
        if (strcmp(current->name, name) == 0) { return current; }
        current = current->next;
    }
    return NULL;
}

Result add_edge(Graph *graph, Vertex *src, Vertex *dst, int attitude) {
    if (src == NULL || dst == NULL) { return FAILURE; }

    Edge *new_edge = (Edge *)calloc(1, sizeof(Edge));
    new_edge->attitude = attitude;
    new_edge->dest = dst;
    new_edge->next = src->connections;
    src->connections = new_edge;

    return SUCCESS;
}

Result remove_vertex(Graph *graph, Vertex *src) {
    if (src == NULL) { return FAILURE; }

    // Remove incoming edges to the removed vertex
    Vertex *vertex = graph->vertices;
    while (vertex != NULL) {
        if (vertex != src) { remove_edge(graph, vertex, src); }
        vertex = vertex->next;
    }

    // Update the prev and next pointers of the linked list
    if (src->prev != NULL) {
        src->prev->next = src->next;
    } else {
        graph->vertices = src->next;
    }

    if (src->next != NULL) { src->next->prev = src->prev; }

    // Update the id of the remaining tail nodes
    vertex = graph->vertices;
    while (vertex != NULL) {
        if (vertex->id >= src->id) vertex->id--;
        vertex = vertex->next;
    }

    // Free the outgoing edges and the vertex itself
    Edge *current_edge = src->connections;
    while (current_edge != NULL) {
        Edge *next_edge = current_edge->next;
        free(current_edge);
        current_edge = next_edge;
    }

    free(src->name);
    free(src);
    graph->size--;

    return SUCCESS;
}

Result remove_edge(Graph *graph, Vertex *src, Vertex *dst) {
    if (src == NULL || dst == NULL) { return FAILURE; }

    Edge *current_edge = src->connections;
    Edge *prev_edge = NULL;

    while (current_edge != NULL) {
        if (current_edge->dest == dst) {
            if (prev_edge != NULL) {
                prev_edge->next = current_edge->next;
            } else {
                src->connections = current_edge->next;
            }
            free(current_edge);
            return SUCCESS;
        }
        prev_edge = current_edge;
        current_edge = current_edge->next;
    }

    return FAILURE;
}

Result set_vertex_name(Graph *graph, Vertex *src, const char *new_name) {
    if (src == NULL || find_vertex(graph, new_name) != NULL) { return FAILURE; }

    free(src->name);
    src->name = strdup(new_name);

    return SUCCESS;
}

Result set_edge_attitude(Graph *graph, Vertex *src, Vertex *dst, int new_attitude) {
    if (src == NULL || dst == NULL) { return FAILURE; }

    Edge *current_edge = src->connections;
    while (current_edge != NULL) {
        if (current_edge->dest == dst) {
            current_edge->attitude = new_attitude;
            return SUCCESS;
        }
        current_edge = current_edge->next;
    }

    return FAILURE;
}

#define max(a, b) (a > b ? a : b)
#define min(a, b) (a < b ? a : b)

void fprint_matrix(FILE *stream, Graph *graph) {
    if (graph->size == 0) { return; }

    // Determine the maximum length of the vertex names
    int max_name_len = 0;
    Vertex *current_vertex = graph->vertices;
    while (current_vertex != NULL) {
        int name_len = strlen(current_vertex->name);
        if (name_len > max_name_len) { max_name_len = name_len; }
        current_vertex = current_vertex->next;
    }
    max_name_len = max(max_name_len, 3);  // strlen("-10")

    // Print the header
    fprintf(stream, "%*s ", max_name_len, "");
    current_vertex = graph->vertices;
    while (current_vertex != NULL) {
        fprintf(stream, "%*s ", max_name_len, current_vertex->name);
        current_vertex = current_vertex->next;
    }
    fprintf(stream, "\n");

    // Print the matrix body
    current_vertex = graph->vertices;
    while (current_vertex != NULL) {
        fprintf(stream, "%*s ", max_name_len, current_vertex->name);
        Vertex *dest_vertex = graph->vertices;
        while (dest_vertex != NULL) {
            Edge *current_edge = current_vertex->connections;
            bool found = false;
            while (current_edge != NULL) {
                if (current_edge->dest == dest_vertex) {
                    fprintf(stream, "%*d ", max_name_len, current_edge->attitude);
                    found = true;
                    break;
                }
                current_edge = current_edge->next;
            }
            if (!found) { fprintf(stream, "%*s ", max_name_len, ""); }
            dest_vertex = dest_vertex->next;
        }
        fprintf(stream, "\n");
        current_vertex = current_vertex->next;
    }
}

void fprint_adj_list(FILE *stream, Graph *graph) {
    Vertex *current_vertex = graph->vertices;
    while (current_vertex != NULL) {
        fprintf(stream, "%s: ", current_vertex->name);
        Edge *current_edge = current_vertex->connections;
        while (current_edge != NULL) {
            fprintf(stream, "%s(%d) ", current_edge->dest->name, current_edge->attitude);
            current_edge = current_edge->next;
        }
        fprintf(stream, "\n");
        current_vertex = current_vertex->next;
    }
}

void dfs_potential_friends(Vertex *vertex, Vertex *result_vertex, bool *visited,
                           Graph *result_graph) {
    visited[vertex->id] = true;
    Edge *edge = vertex->connections;

    while (edge) {
        if (edge->attitude > 0 && !visited[edge->dest->id]) {
            Vertex *current_vertex = add_vertex(result_graph, edge->dest->name);
            add_edge(result_graph, result_vertex, current_vertex, edge->attitude);
            dfs_potential_friends(edge->dest, current_vertex, visited, result_graph);
        }
        edge = edge->next;
    }
}

Graph *find_potential_friends(Graph *graph, Vertex *src) {
    Graph *result_graph = calloc(1, sizeof(Graph));
    Vertex *result_scr = add_vertex(result_graph, src->name);

    bool *visited = calloc(graph->size, sizeof(bool));
    dfs_potential_friends(src, result_scr, visited, result_graph);
    free(visited);

    return result_graph;
}

Graph *shortest_path_bellman_ford(Graph *graph, Vertex *src, Vertex *dst) {
    int *distances = calloc(graph->size, sizeof(int));
    Vertex **predecessors = calloc(graph->size, sizeof(Vertex *));
    for (size_t i = 0; i < graph->size; i++) {
        distances[i] = INT_MAX;
        predecessors[i] = NULL;
    }
    distances[src->id] = 0;

    for (size_t i = 0; i < graph->size - 1; i++) {
        Vertex *vertex = graph->vertices;
        while (vertex) {
            Edge *edge = vertex->connections;
            while (edge) {
                if (distances[vertex->id] != INT_MAX && distances[edge->dest->id] > distances[vertex->id] + edge->attitude) {
                    distances[edge->dest->id] = distances[vertex->id] + edge->attitude;
                    predecessors[edge->dest->id] = vertex;
                }
                edge = edge->next;
            }
            vertex = vertex->next;
        }
    }

    Vertex *current_vertex = graph->vertices;
    while (current_vertex != NULL) {
        Edge *current_edge = current_vertex->connections;
        while (current_edge != NULL) {
            if (distances[current_vertex->id] != INT_MAX && distances[current_edge->dest->id] > distances[current_vertex->id] + current_edge->attitude) {
                free(distances);
                free(predecessors);
                printf("Error: negative cycle detected\n");
                return NULL;
            }
            current_edge = current_edge->next;
        }
        current_vertex = current_vertex->next;
    }

    Graph *result_graph = NULL;
    if (distances[dst->id] != INT_MAX) {
        result_graph = calloc(1, sizeof(Graph));
        Vertex *current_vertex = dst;
        while (current_vertex != src) {
            Vertex *predecessor_vertex = predecessors[current_vertex->id];
            Vertex *added_dst = add_vertex(result_graph, current_vertex->name);
            Vertex *added_src = add_vertex(result_graph, predecessor_vertex->name);
            add_edge(result_graph, added_src, added_dst, distances[current_vertex->id] - distances[predecessor_vertex->id]);
            current_vertex = predecessors[current_vertex->id];
        }
    }

    free(distances);
    free(predecessors);
    return result_graph;
}

void strongconnect(Vertex *v, int *index, int *indices, int *lowlinks, bool *onStack, Vertex **array, int *arrayTop) {
    // Set the depth index for v to the smallest unused index
    indices[v->id] = *index;
    lowlinks[v->id] = *index;
    (*index)++;
    array[(*arrayTop)++] = v;
    onStack[v->id] = true;

    // Consider successors of v
    Edge *e;
    for (e = v->connections; e != NULL; e = e->next) {
        if (e->attitude <= 0) continue;
        Vertex *w = e->dest;
        if (indices[w->id] == -1) {
            // Successor w has not yet been visited; recurse on it
            strongconnect(w, index, indices, lowlinks, onStack, array, arrayTop);
            lowlinks[v->id] = min(lowlinks[v->id], lowlinks[w->id]);
        } else if (onStack[w->id]) {
            // Successor w is in the current SCC
            lowlinks[v->id] = min(lowlinks[v->id], indices[w->id]);
        }
    }

    // If v is a root node, generate an SCC
    if (lowlinks[v->id] == indices[v->id]) {
        Vertex *w;
        do {
            (*arrayTop)--;
            w = array[*arrayTop];
            onStack[w->id] = false;
            // Add w to current SCC
            printf("%s ", w->name);
        } while (w != v);
        printf("\n");
    }
}

void find_strongly_connected_components(Graph *graph) {
    int index = 0;
    int indices[graph->size];
    int lowlinks[graph->size];
    bool onStack[graph->size];
    Vertex *array[graph->size];
    int arrayTop = 0;
    Vertex *v;

    // Initialize all vertices as unvisited
    for (v = graph->vertices; v != NULL; v = v->next) {
        indices[v->id] = -1;
        lowlinks[v->id] = -1;
        onStack[v->id] = false;
    }

    // Perform a DFS on all unvisited vertices
    for (v = graph->vertices; v != NULL; v = v->next) {
        if (indices[v->id] == -1) {
            strongconnect(v, &index, indices, lowlinks, onStack, array, &arrayTop);
        }
    }
}
