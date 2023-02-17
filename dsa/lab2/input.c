#include <stdio.h>
#include <stdlib.h>

void clear_input() {
    char c;
    while ((c = getchar()) != '\n' && c != EOF);
}

int get_int(const char *msg, int *a) {
    while (1) {
        printf("%s", msg);
        int result = scanf("%d", a);
        if (result == 1) return 0;
        if (result != 0) return -1;
        printf("Invalid input, try again!\n");
        clear_input();
    }
}

int get_non_negative_int(const char *msg, int *a) {
    while (1) {
        if (get_int(msg, a)) return -1;
        if (*a >= 0) return 0;
        printf("Number should be non negative, try again!\n");
    }
}

int get_double(const char *msg, double *a) {
    while (1) {
        printf("%s", msg);
        int result = scanf("%lf", a);
        if (result == 1) return 0;
        if (result != 0) return -1;
        printf("Invalid input, try again!\n");
        clear_input();
    }
}
