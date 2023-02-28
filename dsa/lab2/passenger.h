#ifndef PASSENGER_H_
#define PASSENGER_H_

#include <stdlib.h>

#include "sugar/sugar.h"

typedef struct {
    char *name;
    size_t arrival_time;
    size_t service_time;
} Passenger;

void destroy_passenger(Passenger passenger);
Error parse_passenger(char **str, Passenger *passenger);

void fprint_passenger(FILE *stream, Passenger *passenger);
void print_passenger(Passenger *passenger);

#endif  // PASSENGER_H_
