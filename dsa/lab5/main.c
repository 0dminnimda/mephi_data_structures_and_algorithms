#include <ctype.h>
#include <stdio.h>
#include <string.h>

#include "common/input.h"
#include "common/time.h"
#include "graph.c"

#define IS_COMMAND(input, name, short_name) \
    (strcmp(input, name) == 0 || strcmp(input, short_name) == 0)

#define SCAN(n, ...)                                                                \
    ((scanf(__VA_ARGS__) != n) ? (clear(), printf("Error: invalid input\n"), false) \
                               : (clear(), true))

#define SCAN1(...) SCAN(1, __VA_ARGS__)

// #define SCAN(n, ...)                      \
//     if (scanf(__VA_ARGS__) != n) {        \
//         clear();                          \
//         printf("Error: invalid input\n"); \
//         continue;                         \
//     } else {                              \
//         clear();                          \
//     }

void clear() {
    int c;
    while ((c = getchar()) != EOF && strchr("\r\n", c) == NULL) {}
}

char *strip(char *str) {
    while (*str && isspace(*str)) str++;
    char *result = str;
    while (*str && !isspace(*str)) str++;
    *str = '\0';
    return result;
}

int main() {
    Graph *graph = calloc(1, sizeof(Graph));
    char input[100];
    input[0] = '\0';

    bool run = true;
    while (run) {
        printf("\033[32m");  // set text color to green
        printf(
            "Enter command "
            "(add/remove/connect/disconnect/change_name/change_attitude/"
            "output_mat/output_list/dump_dot/image/import_file/reset/quit/"
            "friends/shortest_path/strong_components):\n");
        printf("\033[0m");  // reset text color to default
        printf("> ");

        int result = scanf("%99[^\r\n]", input);
        clear();

        if (result != 1 && result != 0) {
            run = false;
        } else if (IS_COMMAND(input, "add", "a")) {
            printf("Enter name: ");
            char *name = read_line();

            Vertex *v = add_vertex(graph, name);
            if (v == NULL) { printf("Error: could not add vertex\n"); }

            free(name);
        } else if (IS_COMMAND(input, "remove", "r")) {
            printf("Enter name: ");
            char *name = read_line();

            Vertex *v = find_vertex(graph, name);
            if (v == NULL) {
                printf("Error: vertex not found\n");
            } else {
                Result r = remove_vertex(graph, v);
                if (r == FAILURE) { printf("Error: could not delete vertex\n"); }
            }

            free(name);
        } else if (IS_COMMAND(input, "connect", "c")) {
            printf("Enter source name: ");
            char *name1 = read_line();

            printf("Enter destination name: ");
            char *name2 = read_line();

            printf("Enter attitude: ");
            int attitude;
            if (!SCAN1("%d", &attitude) || attitude > 10 || attitude < -10) {
                printf("Error: invalid attitude, must be a number between -10 and 10\n");
            } else {
                Vertex *v1 = find_vertex(graph, name1);
                if (v1 == NULL) {
                    printf("Error: source vertex not found\n");
                } else {
                    Vertex *v2 = find_vertex(graph, name2);
                    if (v2 == NULL) {
                        printf("Error: destination vertex not found\n");
                    } else {
                        Result r = add_edge(graph, v1, v2, attitude);
                        if (r == FAILURE) { printf("Error: could not connect\n"); }
                    }
                }
            }

            free(name1);
            free(name2);
        } else if (IS_COMMAND(input, "disconnect", "d")) {
            printf("Enter source name: ");
            char *name1 = read_line();

            printf("Enter destination name: ");
            char *name2 = read_line();

            Vertex *v1 = find_vertex(graph, name1);
            if (v1 == NULL) {
                printf("Error: source vertex not found\n");
            } else {
                Vertex *v2 = find_vertex(graph, name2);
                if (v2 == NULL) {
                    printf("Error: destination vertex not found\n");
                } else {
                    Result r = remove_edge(graph, v1, v2);
                    if (r == FAILURE) { printf("Error: could not disconnect\n"); }
                }
            }

            free(name1);
            free(name2);
        } else if (IS_COMMAND(input, "change_name", "cn")) {
            printf("Enter name: ");
            char *name = read_line();

            printf("Enter new name: ");
            char *new_name = read_line();

            Vertex *v = find_vertex(graph, name);
            if (v == NULL) {
                printf("Error: vertex not found\n");
            } else {
                Result r = set_vertex_name(graph, v, new_name);
                if (r == FAILURE) { printf("Error: could not change name\n"); }
            }

            free(name);
            free(new_name);
        } else if (IS_COMMAND(input, "change_attitude", "ca")) {
            printf("Enter source name: ");
            char *name1 = read_line();

            printf("Enter destination name: ");
            char *name2 = read_line();

            printf("Enter attitude: ");
            int attitude = 0;
            if (!SCAN1("%d", &attitude) || attitude > 10 || attitude < -10) {
                printf("Error: invalid attitude, must be a number between -10 and 10\n");
            } else {
                Vertex *v1 = find_vertex(graph, name1);
                if (v1 == NULL) {
                    printf("Error: source vertex not found\n");
                } else {
                    Vertex *v2 = find_vertex(graph, name2);
                    if (v2 == NULL) {
                        printf("Error: destination vertex not found\n");
                    } else {
                        Result r = set_edge_attitude(graph, v1, v2, attitude);
                        if (r == FAILURE) {
                            printf("Error: could not change attitude\n");
                        }
                    }
                }
            }

            free(name1);
            free(name2);
        } else if (IS_COMMAND(input, "output_mat", "om")) {
            fprint_matrix(stdout, graph);
        } else if (IS_COMMAND(input, "output_list", "ol")) {
            fprint_adj_list(stdout, graph);
        } else if (IS_COMMAND(input, "friends", "f")) {
            printf("Enter name: ");
            char *name = read_line();

            Vertex *v = find_vertex(graph, name);
            if (v == NULL) {
                printf("Error: vertex not found\n");
            } else {
                Graph *friends = find_potential_friends(graph, v);
                if (friends == NULL) {
                    printf("Error: could not find friends\n");
                } else {
                    fprint_adj_list(stdout, friends);
                    free_graph(friends);
                }
            }

            free(name);
        } else if (IS_COMMAND(input, "shortest_path", "sp")) {
            printf("Enter source name: ");
            char *name1 = read_line();

            printf("Enter destination name: ");
            char *name2 = read_line();

            Vertex *v1 = find_vertex(graph, name1);
            if (v1 == NULL) {
                printf("Error: source vertex not found\n");
            } else {
                Vertex *v2 = find_vertex(graph, name2);
                if (v2 == NULL) {
                    printf("Error: destination vertex not found\n");
                } else {
                    Graph *path = shortest_path_bellman_ford(graph, v1, v2);
                    if (path == NULL) {
                        printf("Error: could not find shortest path\n");
                    } else {
                        fprint_adj_list(stdout, path);
                        free_graph(path);
                    }
                }
            }

            free(name1);
            free(name2);
        } else if (IS_COMMAND(input, "strong_components", "sc")) {
            find_strongly_connected_components(graph);
            // Vertex *components[graph->size];
            // size_t num_components = 0;
            // find_components(graph, components, &num_components);
        
            // printf("Number of components: %zu\n", num_components);
            // for (size_t i = 0; i < num_components; i++) {
            //     printf("Component %zu: ", i+1);
            //     for (size_t j = 0; j < graph->size; j++) {
            //         if (components[i][j].name != NULL) {
            //             printf("%s ", components[i][j].name);
            //         }
            //     }
            //     printf("\n");
            //     free(components[i]);
            // }

            // } else if (IS_COMMAND(input, "dump_dot", "dd")) {
            //     printf("Enter filename: ");
            //     char *filename = read_line();

            //     dump_dot(graph, strip(filename));

            //     free(filename);
            // } else if (IS_COMMAND(input, "image", "i")) {
            //     printf("Enter filename: ");
            //     char *filename = read_line();

            //     to_image(graph, strip(filename));

            //     free(filename);
            // } else if (IS_COMMAND(input, "import_file", "if")) {
            //     printf("Enter filename: ");
            //     char *filename = read_line();

            //     import(graph, strip(filename));

            //     free(filename);
        } else if (IS_COMMAND(input, "reset", "r")) {
            free_graph(graph);
            graph = calloc(1, sizeof(Graph));
        } else if (IS_COMMAND(input, "quit", "q")) {
            run = false;
        } else {
            printf("\033[0;31m");  // set color to red
            printf("Yo mama is so stpid!!! >:(\n");
            printf("Ter aino command '%s'\n", input);
            printf("\033[0m");  // reset color to default
        }
    }

    free_graph(graph);
    return 0;
}
