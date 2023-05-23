#include "graph.h"

#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void free_graph(Graph *graph) {
    // Vertex *current_vertex = graph->vertices;
    // while (current_vertex != NULL) {
    //     Vertex *next_vertex = current_vertex->next;
    //     remove_vertex(graph, current_vertex);
    //     current_vertex = next_vertex;
    // }
    // free(graph);

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

// void copy_graph(Graph *graph) {
//     Graph *result = calloc(sizeof(Graph));
//     Vertex *current_vertex = graph->vertices;
//     while (current_vertex != NULL) {
//         Edge *current_edge = current_vertex->connections;
//         while (current_edge != NULL) {
//             Edge *next_edge = current_edge->next;
//             free(current_edge);
//             current_edge = next_edge;
//         }
//         Vertex *next_vertex = current_vertex->next;
//         free(current_vertex->name);
//         free(current_vertex);
//         current_vertex = next_vertex;
//     }
//     free(graph);
// }

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
    vertex = src->next;
    while (vertex != NULL) {
        vertex->id--;
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

    // current_vertex = dst;
    // while (current_vertex != NULL) {
    //     Edge *current_edge = current_vertex->connections;
    //     while (current_edge != NULL) {
    //         if (predecessor[current_vertex - graph->vertices] == current_edge->dest) {
    //             add_edge(shortest_path_graph,
    //                      find_vertex(shortest_path_graph, current_vertex->name),
    //                      find_vertex(shortest_path_graph, current_edge->dest->name),
    //                      current_edge->attitude);
    //             break;
    //         }
    //         current_edge = current_edge->next;
    //     }
    //     current_vertex = predecessor[current_vertex - graph->vertices];
    // }
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

// void strongconnect(Vertex *v, int *index, int *lowlink, bool *onStack, Stack *stack) {
//     // Set the depth index for v to the smallest unused index
//     v->index = *index;
//     v->lowlink = *index;
//     (*index)++;
//     stack_push(stack, v);
//     onStack[v->id] = true;

//     // Consider successors of v
//     Edge *e;
//     for (e = v->connections; e != NULL; e = e->next) {
//         Vertex *w = e->dest;
//         if (w->index == -1) {
//             // Successor w has not yet been visited; recurse on it
//             strongconnect(w, index, lowlink, onStack, stack);
//             v->lowlink = MIN(v->lowlink, w->lowlink);
//         } else if (onStack[w->id]) {
//             // Successor w is in stack S and hence in the current SCC
//             v->lowlink = MIN(v->lowlink, w->index);
//         }
//     }

//     // If v is a root node, pop the stack and generate an SCC
//     if (v->lowlink == v->index) {
//         Vertex *w;
//         do {
//             w = stack_pop(stack);
//             onStack[w->id] = false;
//             // Add w to current SCC
//             printf("%s ", w->name);
//         } while (w != v);
//         printf("\n");
//     }
// }

// void find_strongly_connected_components(Graph *graph) {
//     int index = 0;
//     bool onStack[graph->size];
//     Stack *stack = stack_create();
//     Vertex *v;

//     // Initialize all vertices as unvisited
//     for (v = graph->vertices; v != NULL; v = v->next) {
//         v->index = -1;
//         v->lowlink = -1;
//         onStack[v->id] = false;
//     }

//     // Perform a DFS on all unvisited vertices
//     for (v = graph->vertices; v != NULL; v = v->next) {
//         if (v->index == -1) {
//             strongconnect(v, &index, v->lowlink, onStack, stack);
//         }
//     }

//     stack_destroy(stack);
// }


// void dfs_partition(Vertex *vertex, bool *visited, Graph *result_graph, int component_id) {
//     visited[vertex->id] = true;
//     Vertex *result_vertex = add_vertex(result_graph, vertex->name);
//     Edge *edge = vertex->connections;

//     while (edge) {
//         if (edge->attitude > 0 && !visited[edge->dest->id]) {
//             add_edge(result_graph, result_vertex, find_vertex(result_graph, edge->dest->name), edge->attitude);
//             dfs_partition(edge->dest, visited, result_graph, component_id);
//         }
//         edge = edge->next;
//     }
// }

// Graph *partition_positive_relationships(Graph *graph) {
//     Graph *result_graph = calloc(1, sizeof(Graph));
//     bool *visited = calloc(graph->size, sizeof(bool));

//     Vertex *vertex = graph->vertices;
//     while (vertex) {
//         if (!visited[vertex->id]) {
//             dfs_partition(vertex, visited, result_graph, vertex->id);
//         }
//         vertex = vertex->next;
//     }

//     free(visited);
//     return result_graph;
// }


// Vertex **find_potential_friends(Graph *graph, Vertex *src) {
//     Vertex **potential_friends = (Vertex **)calloc(graph->size, sizeof(Vertex *));
//     size_t num_friends = 0;

//     Vertex *current_vertex = graph->vertices;
//     while (current_vertex != NULL) {
//         if (current_vertex != src) {
//             Edge *current_edge = current_vertex->connections;
//             while (current_edge != NULL) {
//                 if (current_edge->dest == src) {
//                     potential_friends[num_friends++] = current_vertex;
//                     break;
//                 }
//                 current_edge = current_edge->next;
//             }
//         }
//         current_vertex = current_vertex->next;
//     }

//     potential_friends[num_friends] = NULL;
//     return potential_friends;
// }

// Graph *shortest_path_bellman_ford(Graph *graph, Vertex *src, Vertex *dst) {
//     // Initialize the distance array
//     int *distance = (int *)calloc(graph->size, sizeof(int));
//     Vertex **predecessor = (Vertex **)calloc(graph->size, sizeof(Vertex *));
//     Vertex *current_vertex = graph->vertices;
//     while (current_vertex != NULL) {
//         distance[current_vertex - graph->vertices] = INT_MAX;
//         predecessor[current_vertex - graph->vertices] = NULL;
//         current_vertex = current_vertex->next;
//     }
//     distance[src - graph->vertices] = 0;

//     // Relax edges repeatedly
//     for (size_t i = 0; i < graph->size - 1; i++) {
//         current_vertex = graph->vertices;
//         while (current_vertex != NULL) {
//             Edge *current_edge = current_vertex->connections;
//             while (current_edge != NULL) {
//                 int new_distance =
//                     distance[current_vertex - graph->vertices] +
//                     current_edge->attitude;
//                 if (new_distance < distance[current_edge->dest - graph->vertices]) {
//                     distance[current_edge->dest - graph->vertices] = new_distance;
//                     predecessor[current_edge->dest - graph->vertices] = current_vertex;
//                 }
//                 current_edge = current_edge->next;
//             }
//             current_vertex = current_vertex->next;
//         }
//     }

//     // Check for negative cycles
//     current_vertex = graph->vertices;
//     while (current_vertex != NULL) {
//         Edge *current_edge = current_vertex->connections;
//         while (current_edge != NULL) {
//             int new_distance =
//                 distance[current_vertex - graph->vertices] + current_edge->attitude;
//             if (new_distance < distance[current_edge->dest - graph->vertices]) {
//                 free(distance);
//                 free(predecessor);
//                 return NULL;  // Negative cycle detected
//             }
//             current_edge = current_edge->next;
//         }
//         current_vertex = current_vertex->next;
//     }

//     // Build the shortest path graph
//     Graph *shortest_path_graph = (Graph *)calloc(1, sizeof(Graph));
//     shortest_path_graph->vertices = NULL;
//     shortest_path_graph->size = 0;

//     current_vertex = dst;
//     while (current_vertex != NULL) {
//         add_vertex(shortest_path_graph, current_vertex->name);
//         current_vertex = predecessor[current_vertex - graph->vertices];
//     }

//     current_vertex = dst;
//     while (current_vertex != NULL) {
//         Edge *current_edge = current_vertex->connections;
//         while (current_edge != NULL) {
//             if (predecessor[current_vertex - graph->vertices] == current_edge->dest) {
//                 add_edge(shortest_path_graph,
//                          find_vertex(shortest_path_graph, current_vertex->name),
//                          find_vertex(shortest_path_graph, current_edge->dest->name),
//                          current_edge->attitude);
//                 break;
//             }
//             current_edge = current_edge->next;
//         }
//         current_vertex = predecessor[current_vertex - graph->vertices];
//     }

//     free(distance);
//     free(predecessor);
//     return shortest_path_graph;
// }

// Graph *partition_connected_components(Graph *graph) {
//     Graph *components = (Graph *)calloc(1, sizeof(Graph));
//     components->vertices = NULL;
//     components->size = 0;

//     Vertex *current_vertex = graph->vertices;
//     while (current_vertex != NULL) {
//         if (find_vertex(components, current_vertex->name) == NULL) {
//             // Perform a BFS to find all vertices in the same component
//             Graph *component = (Graph *)calloc(1, sizeof(Graph));
//             component->vertices = NULL;
//             component->size = 0;

//             Queue *queue = create_queue();
//             enqueue(queue, current_vertex);

//             while (!is_queue_empty(queue)) {
//                 Vertex *current_vertex = dequeue(queue);
//                 if (find_vertex(component, current_vertex->name) == NULL) {
//                     add_vertex(component, current_vertex->name);
//                     Edge *current_edge = current_vertex->connections;
//                     while (current_edge != NULL) {
//                         enqueue(queue, current_edge->dest);
//                         current_edge = current_edge->next;
//                     }
//                 }
//             }

//             free_queue(queue);
//             add_vertex(components, component->vertices->name);
//             free_graph(component);
//         }
//         current_vertex = current_vertex->next;
//     }

//     return components;
// }

// Vertex **find_potential_friends(Graph *graph, Vertex *src) {
//     Vertex **potential_friends = (Vertex **)calloc(1, sizeof(Vertex *) * graph->size);
//     size_t num_friends = 0;

//     Vertex *current_vertex = graph->vertices;
//     while (current_vertex != NULL) {
//         if (current_vertex != src) {
//             Edge *current_edge = current_vertex->connections;
//             while (current_edge != NULL) {
//                 if (current_edge->dest == src) {
//                     potential_friends[num_friends++] = current_vertex;
//                     break;
//                 }
//                 current_edge = current_edge->next;
//             }
//         }
//         current_vertex = current_vertex->next;
//     }

//     potential_friends[num_friends] = NULL;
//     return potential_friends;
// }

// Graph *shortest_path_bellman_ford(Graph *graph, Vertex *src, Vertex *dst) {
//     int *distances = (int *)calloc(1, sizeof(int) * graph->size);
//     Vertex **predecessors = (Vertex **)calloc(1, sizeof(Vertex *) * graph->size);

//     // Initialize distances and predecessors
//     Vertex *current_vertex = graph->vertices;
//     while (current_vertex != NULL) {
//         distances[current_vertex - graph->vertices] = INT_MAX;
//         predecessors[current_vertex - graph->vertices] = NULL;
//         current_vertex = current_vertex->next;
//     }
//     distances[src - graph->vertices] = 0;

//     // Relax edges repeatedly
//     for (size_t i = 0; i < graph->size - 1; i++) {
//         current_vertex = graph->vertices;
//         while (current_vertex != NULL) {
//             Edge *current_edge = current_vertex->connections;
//             while (current_edge != NULL) {
//                 int new_distance =
//                     distances[current_vertex - graph->vertices] +
//                     current_edge->attitude;
//                 if (new_distance < distances[current_edge->dest - graph->vertices]) {
//                     distances[current_edge->dest - graph->vertices] = new_distance;
//                     predecessors[current_edge->dest - graph->vertices] =
//                     current_vertex;
//                 }
//                 current_edge = current_edge->next;
//             }
//             current_vertex = current_vertex->next;
//         }
//     }

//     // Check for negative cycles
//     current_vertex = graph->vertices;
//     while (current_vertex != NULL) {
//         Edge *current_edge = current_vertex->connections;
//         while (current_edge != NULL) {
//             int new_distance =
//                 distances[current_vertex - graph->vertices] + current_edge->attitude;
//             if (new_distance < distances[current_edge->dest - graph->vertices]) {
//                 free(distances);
//                 free(predecessors);
//                 return NULL;  // Negative cycle detected
//             }
//             current_edge = current_edge->next;
//         }
//         current_vertex = current_vertex->next;
//     }

//     // Build the shortest path graph
//     Graph *shortest_path_graph = (Graph *)calloc(1, sizeof(Graph));
//     shortest_path_graph->vertices = NULL;
//     shortest_path_graph->size = 0;

//     current_vertex = dst;
//     while (current_vertex != NULL) {
//         add_vertex(shortest_path_graph, current_vertex->name);
//         current_vertex = predecessors[current_vertex - graph->vertices];
//     }

//     current_vertex = dst;
//     while (current_vertex != NULL) {
//         Edge *current_edge = current_vertex->connections;
//         while (current_edge != NULL) {
//             if (predecessors[current_edge->dest - graph->vertices] == current_vertex) {
//                 add_edge(shortest_path_graph,
//                          find_vertex(shortest_path_graph, current_vertex->name),
//                          find_vertex(shortest_path_graph, current_edge->dest->name),
//                          current_edge->attitude);
//             }
//             current_edge = current_edge->next;
//         }
//         current_vertex = predecessors[current_vertex - graph->vertices];
//     }

//     free(distances);
//     free(predecessors);
//     return shortest_path_graph;
// }

// Graph *partition_connected_components(Graph *graph) {
//     Graph *components = (Graph *)calloc(1, sizeof(Graph));
//     components->vertices = NULL;
//     components->size = 0;

//     Vertex *current_vertex = graph->vertices;
//     while (current_vertex != NULL) {
//         if (find_vertex(components, current_vertex->name) == NULL) {
//             // Perform a BFS to find all vertices in the same component
//             Graph *component = (Graph *)calloc(1, sizeof(Graph));
//             component->vertices = NULL;
//             component->size = 0;

//             Vertex **queue = (Vertex **)calloc(graph->size, sizeof(Vertex *));
//             size_t front = 0, rear = 0;
//             queue[rear++] = current_vertex;

//             while (front < rear) {
//                 Vertex *current = queue[front++];
//                 if (find_vertex(component, current->name) == NULL) {
//                     add_vertex(component, current->name);
//                     Vertex *neighbor = current->connections->dest;
//                     while (neighbor != NULL) {
//                         if (find_vertex(component, neighbor->name) == NULL) {
//                             queue[rear++] = neighbor;
//                         }
//                         neighbor = neighbor->connections->next;
//                     }
//                 }
//             }

//             free(queue);
//             add_vertex(components, current_vertex->name);
//             current_vertex = current_vertex->next;

//             // Add all vertices in the component to the new graph
//             Vertex *component_vertex = component->vertices;
//             while (component_vertex != NULL) {
//                 add_vertex(components, component_vertex->name);
//                 component_vertex = component_vertex->next;
//             }

//             // Add all edges in the component to the new graph
//             component_vertex = component->vertices;
//             while (component_vertex != NULL) {
//                 Edge *current_edge = component_vertex->connections;
//                 while (current_edge != NULL) {
//                     add_edge(components, find_vertex(components,
//                     component_vertex->name),
//                              find_vertex(components, current_edge->dest->name),
//                              current_edge->attitude);
//                     current_edge = current_edge->next;
//                 }
//                 component_vertex = component_vertex->next;
//             }

//             free_graph(component);
//         } else {
//             current_vertex = current_vertex->next;
//         }
//     }

//     return components;
// }

// // Define additional functions to support the required operations

// // Traverse the graph in depth to determine the list of people that the specified
// person could potentially be friends with
// // The chain of pairs of acquaintances should have no pair of people who have a
// negative relationship with each other.
// // This can be implemented using a depth-first search algorithm.
// void dfs(Vertex *src, Vertex *dst, int *visited, Vertex **friends, int *count) {
//     visited[src->name] = 1; // Mark the current vertex as visited
//     Edge *edge = src->connections;
//     while (edge != NULL) {
//         if (edge->attitude >= 0 && visited[edge->dest->name] == 0) {
//             if (edge->dest == dst) {
//                 friends[*count] = edge->dest;
//                 (*count)++;
//             } else {
//                 dfs(edge->dest, dst, visited, friends, count);
//             }
//         }
//         edge = edge->next;
//     }
// }

// Vertex **find_potential_friends(Graph *graph, Vertex *src) {
//     Vertex **friends = calloc(graph->size, sizeof(Vertex *));
//     int visited[graph->size];
//     memset(visited, 0, graph->size * sizeof(int));
//     int count = 0;
//     Vertex *vertex = graph->vertices;
//     while (vertex != NULL) {
//         if (vertex != src) {
//             dfs(src, vertex, visited, friends, &count);
//         }
//         vertex = vertex->next;
//     }
//     friends[count] = NULL;
//     return friends;
// }

// // Find the shortest path between two given vertices of the graph using the
// Bellman-Ford algorithm.
// // This algorithm can handle negative edge weights.
// Graph *shortest_path_bellman_ford(Graph *graph, Vertex *src, Vertex *dst) {
//     int size = graph->size;
//     int dist[size];
//     Vertex *prev[size];
//     Vertex *vertex = graph->vertices;
//     // Initialize the distance array and predecessor array
//     for (int i = 0; i < size; i++) {
//         dist[i] = INT_MAX;
//         prev[i] = NULL;
//     }
//     // Set the distance to the source vertex as 0
//     dist[src->name] = 0;
//     // Relax all edges V-1 times
//     for (int i = 1; i < size; i++) {
//         Edge *edge = graph->vertices->connections;
//         while (edge != NULL) {
//             int u = edge->src->name;
//             int v = edge->dest->name;
//             int weight = edge->attitude;
//             if (dist[u] != INT_MAX && dist[u] + weight < dist[v]) {
//                 dist[v] = dist[u] + weight;
//                 prev[v] = edge->src;
//             }
//             edge = edge->next;
//         }
//     }
//     // Check for negative edge cycles
//     Edge *edge = graph->vertices->connections;
//     while (edge != NULL) {
//         int u = edge->src->name;
//         int v = edge->dest->name;
//         int weight = edge->attitude;
//         if (dist[u] != INT_MAX && dist[u] + weight < dist[v]) {
//             printf("Graph contains negative edge cycle\n");
//             return NULL;
//         }
//         edge = edge->next;
//     }

//     // Reconstruct the shortest path from src to dst
//     Graph *shortest_path = calloc(1, sizeof(Graph));
//     shortest_path->size = 0;
//     shortest_path->vertices = NULL;

//     Vertex *path_vertex = dst;
//     while (path_vertex != NULL) {
//         Vertex *new_vertex = add_vertex(shortest_path, path_vertex->name);
//         if (shortest_path->vertices == NULL) {
//             shortest_path->vertices = new_vertex;
//         } else {
//             new_vertex->next = shortest_path->vertices;
//             shortest_path->vertices->prev = new_vertex;
//             shortest_path->vertices = new_vertex;
//         }
//         shortest_path->size++;
//         path_vertex = prev[path_vertex->name];
//     }

//     return shortest_path;
// }
