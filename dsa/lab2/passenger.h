#include <stdlib.h>

typedef struct {
    char *name;
    size_t arrival_time;
    size_t service_time;
} Passenger;

Passenger parse_passenger(const char *string);
