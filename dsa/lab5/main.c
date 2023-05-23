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
    Graph *graph = calloc(sizeof(Graph));
    char input[100];
    input[0] = '\0';

    bool run = true;
    while (run) {
        printf("\033[32m");  // set text color to green
        printf(
            "Enter command "
            "(add/delete/find/max_diff/min/traverse/output/visit/dump_dot/"
            "image/import_file/clock_zero/clock_time/reset/quit):\n");
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
        } else if (IS_COMMAND(input, "delete", "d")) {
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
            if (!SCAN1("%d", &attitude)) {
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
        } else if (IS_COMMAND(input, "disconnect", "dc")) {
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
            int attitude;
            if (!SCAN1("%d", &attitude)) {
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
            // } else if (IS_COMMAND(input, "friends", "f")) {
            //     fprint_adj_list(stdout, graph);
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
            graph = calloc(sizeof(Graph));
        } else if (IS_COMMAND(input, "quit", "q")) {
            run = false;
        } else {
            printf("\033[0;31m");  // set color to red
            printf("Yo mama is so stpid!!! >:(\n");
            printf("Ter aino command '%s'\n", input);
            printf("\033[0m");  // reset color to default
        }
    }

    destroy_graph(graph);
    return 0;
}
