int main() {
Graph g = { NULL, 0 };
char input[MAX_NAME_LEN];
int attitude;
Vertex *src, *dst;
Result res;
while (true) {
    printf("Enter command: ");
    scanf("%s", input);

    if (strcmp(input, "add_vertex") == 0) {
        scanf("%s", input);
        add_vertex(&g, input);
    }
    else if (strcmp(input, "add_edge") == 0) {
        scanf("%s", input);
        src = find_vertex(&g, input);
        scanf("%s", input);
        dst = find_vertex(&g, input);
        scanf("%d", &attitude);
        add_edge(&g, src, dst, attitude);
    }
    else if (strcmp(input, "remove_vertex") == 0) {
        scanf("%s", input);
        src = find_vertex(&g, input);
        remove_vertex(&g, src);
    }
    else if (strcmp(input, "remove_edge") == 0) {
        scanf("%s", input);
        src = find_vertex(&g, input);
        scanf("%s", input);
        dst = find_vertex(&g, input);
        remove_edge(&g, src, dst);
    }
    else if (strcmp(input, "set_vertex_name") == 0) {
        scanf("%s", input);
        src = find_vertex(&g, input);
        scanf("%s", input);
        set_vertex_name(&g, src, input);
    }
    else if (strcmp(input, "set_edge_attitude") == 0) {
        scanf("%s", input);
        src = find_vertex(&g, input);
        scanf("%s", input);
        dst = find_vertex(&g, input);
        scanf("%d", &attitude);
        set_edge_attitude(&g, src, dst, attitude);
    }
    else if (strcmp(input, "fprint_matrix") == 0) {
        fprint_matrix(stdout, &g);
    }
    else if (strcmp(input, "fprint_adj_list") == 0) {
        fprint_adj_list(stdout, &g);
    }
    else if (strcmp(input, "find_potential_friends") == 0) {
        scanf("%s", input);
        src = find_vertex(&g, input);
        Vertex **friends = find_potential_friends(&g, src);
        printf("%s's potential friends:\n", src->name);
        for (int i = 0; friends[i] != NULL; i++) {
            printf("%s\n", friends[i]->name);
        }
        free(friends);
    }
    else if (strcmp(input, "shortest_path_bellman_ford") == 0) {
        scanf("%s", input);
