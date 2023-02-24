#include "passenger.h"

#include <errno.h>
#include <string.h>

#include "error.h"
#include "memo.h"
#include "string.h"

// parses Passenger in a format "name/arrival_time/service_time"
Error parse_passenger(char **str, Passenger *passenger) {
    Passenger p = {0};

    char *name = *str = *str + strspn(*str, " ");
    if (*name == '\0') return PARSE_ERROR("Encountered the end of the string");

    char *arrival_time = strchr(*str, '/');
    if (arrival_time++ == NULL) return PARSE_ERROR("No arrival time specified");
    *str = arrival_time;

    TRY(parse_ulong(str, &p.arrival_time))
    CATCH(PARSE_ERROR_TYPE) {
        return PARSE_ERROR("Arrival time is not a valid non-negative number");
    }
    CATCH_N_THROW

    char *service_time = strchr(*str, '/');
    if (service_time++ == NULL) return PARSE_ERROR("No service time specified");
    *str = service_time;

    TRY(parse_ulong(str, &p.service_time))
    CATCH(PARSE_ERROR_TYPE) {
        return PARSE_ERROR("Service time is not a valid non-negative number");
    }
    CATCH_N_THROW

    NEW(p.name, arrival_time - name);
    strncpy(p.name, name, arrival_time - name);

    *passenger = p;
    return OK;
}

void fprint_passenger(FILE *stream, Passenger *passenger) {
    fprintf(stream, "%s %zu %zu", passenger->name, passenger->arrival_time,
            passenger->service_time);
}
