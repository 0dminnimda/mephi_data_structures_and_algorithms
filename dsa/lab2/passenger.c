#include "passenger.h"

#include <errno.h>
#include <string.h>

#include "common/string.h"
#include "sugar/sugar.h"

void destroy_passenger(Passenger passenger) { free(passenger.name); }

// parses Passenger in a format "name/arrival_time/service_time"
Error parse_passenger(char **str, Passenger *passenger) {
    Error error;
    Passenger p = {0};

    char *name = *str = *str + strspn(*str, WHITESPACE);
    if (*name == '\0') return PARSE_ERROR("Encountered the end of the string");
    if (*name == '/') return PARSE_ERROR("No name specified");

    char *arrival_time = strchr(*str, '/');
    if (arrival_time++ == NULL) return PARSE_ERROR("No arrival time specified");
    *str = arrival_time;

    TRY(parse_size_t(str, &p.arrival_time))
    CATCH(PARSE_ERROR_TYPE) {
        return PARSE_ERROR("Arrival time is not a valid non-negative number");
    }
    CATCH_N_THROW

    // "name/123 invalid/123"
    char *whitespace_end = *str = *str + strspn(*str, WHITESPACE);
    if (*whitespace_end != '/')
        return PARSE_ERROR("Arrival time is not a valid non-negative number");

    char *service_time = *str;
    if (service_time++ == NULL) return PARSE_ERROR("No service time specified");
    *str = service_time;

    TRY(parse_size_t(str, &p.service_time))
    CATCH(PARSE_ERROR_TYPE) {
        return PARSE_ERROR("Service time is not a valid non-negative number");
    }
    CATCH_N_THROW

    NEW(p.name, arrival_time - 1 - name);
    strncpy(p.name, name, arrival_time - 1 - name);

    *passenger = p;
    return OK;
}

void fprint_passenger(FILE *stream, Passenger *passenger) {
    fprintf(stream, "%s %zu %zu", passenger->name, passenger->arrival_time,
            passenger->service_time);
}

void print_passenger(Passenger *passenger) {
    fprint_passenger(stdout, passenger);
}

void fprint_passenger_name(FILE *stream, Passenger *passenger) {
    fprintf(stream, "%s", passenger->name);
}

void print_passenger_name(Passenger *passenger) {
    fprint_passenger_name(stdout, passenger);
}
