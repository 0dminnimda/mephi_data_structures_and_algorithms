#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    char name[20];
    int age;
    int height;
    int weight;
} Person;

int compare_name(const void *a, const void *b) {
    Person *p1 = (Person *)a;
    Person *p2 = (Person *)b;
    return strcmp(p1->name, p2->name);
}

int compare_age(const void *a, const void *b) {
    Person *p1 = (Person *)a;
    Person *p2 = (Person *)b;
    return p1->age - p2->age;
}

int compare_height(const void *a, const void *b) {
    Person *p1 = (Person *)a;
    Person *p2 = (Person *)b;
    return p1->height - p2->height;
}

int compare_weight(const void *a, const void *b) {
    Person *p1 = (Person *)a;
    Person *p2 = (Person *)b;
    return p1->weight - p2->weight;
}

void multi_sort(Person *people, int (*comparators[])(const void *, const void *)) {
    int i = 0;
    while (comparators[i] != NULL) {
        qsort(people, 20, sizeof(Person), comparators[i]);
        i++;
    }
}

int main() {
    Person people[] = {
        {"John", 20, 170, 60},
        {"Mary", 19, 165, 55},
        {"Bob", 20, 180, 70},
        {"Mark", 21, 175, 65},
        {"Alice", 19, 160, 50},
        {"Jane", 20, 170, 60},
        {"Tom", 21, 180, 70},
        {"Jerry", 19, 165, 55},
        {"Peter", 20, 170, 60},
        {"Helen", 19, 160, 50},
        {"Kevin", 20, 180, 70},
        {"Lily", 21, 175, 65},
        {"Rose", 19, 165, 55},
        {"Jack", 20, 170, 60},
        {"David", 21, 180, 70},
        {"Mike", 19, 160, 50},
        {"Sandy", 20, 170, 60},
        {"Betty", 19, 165, 55},
        {"Sandy", 20, 170, 60},
        {"Betty", 19, 165, 55},
    };

    int (*comparators[])(const void *, const void *) = {compare_age, compare_weight, compare_height, NULL};

    for (int i = 0; i < 20; i++) {
        printf("%s %d %d %d\n", people[i].name, people[i].age, people[i].height, people[i].weight);
    }
    printf("\n");

    multi_sort(people, comparators);

    for (int i = 0; i < 20; i++) {
        printf("%s %d %d %d\n", people[i].name, people[i].age, people[i].height, people[i].weight);
    }

    return 0;
}
