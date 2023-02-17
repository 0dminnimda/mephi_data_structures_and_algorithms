#include "passenger.h"

#include <string.h>

#include "memo.h"

// parses passenger in a format "name/arrival_time/service_time"
Passenger parse_passenger(char *string) {
    char *first = string + strcspn(string, "/");
    char *second = first + strcspn(first, "/");
    char *end = second + strlen(second);

    Passenger passenger = {string, 0, 0};
    *first = '\0';
    if (first != end) passenger.arrival_time = strtoul(first + 1, NULL, 10);
    if (second != end) passenger.service_time = strtoul(second + 1, NULL, 10);
    return passenger;
}
