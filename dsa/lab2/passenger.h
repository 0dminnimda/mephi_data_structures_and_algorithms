#ifndef PASSENGER_H_
#define PASSENGER_H_

#include <stdlib.h>

#include "error.h"

typedef struct {
    char *name;
    size_t arrival_time;
    size_t service_time;
} Passenger;

Passenger parse_passenger(char *string);

#endif  // PASSENGER_H_
