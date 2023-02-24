

void clear_input();
Error get_int(const char *msg, int *a);
Error get_non_negative_int(const char *msg, int *a);
Error get_double(const char *msg, double *a);


void clear_input() {
    char c;
    while ((c = getchar()) != '\n' && c != EOF);
}

Error get_int(int *a) {
    int result = scanf("%d", a);
    if (result == 1) return OK;
    if (result == EOF) return INPUT_ERROR("End of file");
    clear_input();
    return INPUT_ERROR("Not an integer");
}

Error get_non_negative_int(int *a) {
    if (get_int(stream, a)) return INPUT_ERROR("Not a non negative integer");
    if (*a >= 0) return OK;
}

Error get_double(double *a) {
    int result = scanf("%lf", a);
    if (result == 1) return OK;
    if (result == EOF) return INPUT_ERROR("End of file");
    clear_input();
    return INPUT_ERROR("Not a floating point number");
}
