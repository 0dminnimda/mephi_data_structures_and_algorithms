#include "passenger.h"

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
