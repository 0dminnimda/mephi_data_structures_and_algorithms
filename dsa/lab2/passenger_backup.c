#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    char *name;
    size_t arrival_time;
    size_t service_time;
} Passenger;

// parses passenger in a format "name/arrival_time/service_time"
Passenger parse_passenger(const char *string) {
    Passenger passenger = {0};

    char *first = string + strcspn(string, "/");
    char *second = first + strcspn(first, "/");

    NEW(passenger.name, (first - string + 1) * sizeof(char));
    passenger.name[first - string] = '\0';
    if (first) passenger.arrival_time = strtoul(first + 1, second, 10);
    if (second) passenger.service_time = strtoul(second + 1, NULL, 10);

    return passenger;
}

// // parses passenger in a format "name/arrival_time/service_time"
// Passenger parse_passenger(const char *string) {
//     Passenger passenger = {0};

//     char *end = string + strlen(string);

//     char *first = strchr(string, '/');
//     if (first)
//         ;
//     else
//         first = end;

//     char *second = strchr(first, '/');

//     char *name = NEW(name, (first - string) * sizeof(char))
//     // char *name = strdup(string);
//     return (Passenger){
//         .name = name,
//         .arrival_time = first? strtoul(first + 1, second, 10) : 0,
//         .service_time = second? strtoul(second + 1, NULL, 10) : 0,
//     };
// }

// // parse passenger that looks like that: name/arrival_time/service_time
// // does not change the original string
// // name is a string, arrival_time and service_time are size_t
// Passenger parse_passenger(const char *string) {
//     char *name = strdup(string);

//     char *arrival_time = strchr(name, '/');
//     *arrival_time = '\0';
//     arrival_time++;

//     char *service_time = strchr(arrival_time, '/');
//     *service_time = '\0';
//     service_time++;

//     return (Passenger){
//         .name = name,
//         .arrival_time = strtoul(arrival_time, NULL, 10),
//         .service_time = strtoul(service_time, NULL, 10),
//     };
// }
