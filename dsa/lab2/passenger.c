#include "passenger.h"

#include <errno.h>
#include <string.h>

#include "error.h"
#include "memo.h"
#include "string.h"

// parses passenger in a format "name/arrival_time/service_time"
// and returns an Error if the format is invalid)
// also cannot modify an input string
// thus allocates memory for the name (using NEW)
// and updates the string pointer to point to the end of the passenger
Error parse_passenger(char **str, Passenger *passenger) {
    Passenger p = {0};

    char *name = *str = *str + strspn(*str, " ");

    char *arrival_time = *str = strchr(*str, '/');
    if (arrival_time++ == NULL) return VALUE_ERROR("No arrival time specified");

    TRY(parse_ulong(str, &p.arrival_time))
    CATCH(VALUE_ERROR_TYPE)
        return VALUE_ERROR("Arrival time is not a valid non-negative number");
    CATCH_N_THROW

    char *service_time = *str = strchr(*str, '/');
    if (service_time++ == NULL) return VALUE_ERROR("No service time specified");

    TRY(parse_ulong(str, &p.service_time))
    CATCH(VALUE_ERROR_TYPE)
        return VALUE_ERROR("Service time is not a valid non-negative number");
    CATCH_N_THROW

    NEW(p.name, arrival_time - name);
    strncpy(p.name, name, arrival_time - name);

    *passenger = p;
    return OK;
}
