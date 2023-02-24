#include "passenger.h"

#include <errno.h>
#include <string.h>

#include "input.h"
#include "memo.h"

// parses passenger in a format "name/arrival_time/service_time"
// and returns an Error if the format is invalid)
// also cannot modify an input string
// thus allocates memory for the name (using NEW)
// and updates the string pointer to point to the end of the passenger
Error parse_passenger(char **str, Passenger *passenger) {
    Passenger p = {0};

    char *name = *str = *str + strspn(*str, " ");

    char *arrival_time = strchr(*str, '/');
    if (arrival_time++ == NULL) return INPUT_ERROR("No arrival time specified");

    char *service_time = strchr(arrival_time, '/');
    if (service_time++ == NULL) return INPUT_ERROR("No service time specified");

    p.arrival_time = strtol(arrival_time, str, 10);
    if (p.arrival_time < 0 || *str == arrival_time || errno != 0) {
        return INPUT_ERROR("Arrival time is not a valid non-negative number");
    }

    p.service_time = strtol(service_time, str, 10);
    if (p.service_time < 0 || *str == service_time || errno != 0) {
        return INPUT_ERROR("Service time is not a valid non-negative number");
    }

    NEW(p.name, arrival_time - name);
    strncpy(p.name, name, arrival_time - name);

    *passenger = p;
    return OK;
}
