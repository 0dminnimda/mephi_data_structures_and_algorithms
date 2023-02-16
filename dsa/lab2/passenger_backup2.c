#include <stdlib.h>
#include <stdio.h>
#include <string.h>

typedef struct {
    char *name;
    size_t arrival_time;
    size_t service_time;
} Passenger;

// parse passenger that looks like that: name/arrival_time/service_time
// does not change the original string
// name is a string, arrival_time and service_time are size_t
Passenger parse_passenger(const char *string) {
    Passenger passenger;
    char *token;
    char *rest = string;
    token = strtok_r(rest, "/", &rest);
    passenger.name = token;
    token = strtok_r(rest, "/", &rest);
    passenger.arrival_time = atoi(token);
    token = strtok_r(rest, "/", &rest);
    passenger.service_time = atoi(token);
    return passenger;
}
