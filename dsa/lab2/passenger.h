#ifndef PASSENGER_H_
#define PASSENGER_H_

#include <stdlib.h>

#include "error.h"

typedef struct {
    char *name;
    size_t arrival_time;
    size_t service_time;
} Passenger;

Error parse_passenger(char **str, Passenger *passenger);
void fprint_passenger(FILE *stream, Passenger *passenger);

#endif  // PASSENGER_H_
