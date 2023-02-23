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
// and returns an Error if the format is invalid)
// also cannot modify an input string
// thus allocates memory for the name (using NEW)
// and updates the string pointer to point to the end of the passenger
Error parse_passenger(char **str, Passenger *passenger) {
    char *name = *str + strcspn(*str, "/");
    char *arrival_time = strchr(name, '/');
    if (arrival_time == NULL) return INPUT_ERROR("No arrival time specified");

    char *service_time = strchr(arrival_time, '/');
    if (service_time == NULL) return INPUT_ERROR("No service time specified");

    NEW(passenger->name, arrival_time - name);
    strncpy(passenger->name, name, arrival_time - name);

    char *end;
    passenger->arrival_time = strtol(arrival_time + 1, &end, 10);
    if (end == arrival_time + 1)
        return INPUT_ERROR("Arrival time is not an number");

    passenger->service_time = strtol(service_time + 1, &end, 10);
    if (end == service_time + 1)
        return INPUT_ERROR("Service time is not an number");

    *str = service_time + strlen(service_time);
    return OK;
}
