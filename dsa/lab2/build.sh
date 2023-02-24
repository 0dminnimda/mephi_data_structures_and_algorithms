#!/usr/bin/env bash

MY_CC="cc"
ARGS="input.c passenger.c load_balancer.c queue.c memo.c main.c -o main.out -O3 -g"
# ARGS="input.c memo.c sugar_test.c -o main.out -O3 -g"

if [[ $(which cc &> /dev/null; echo $?) != "0" ]]; then
    echo "I did not find 'cc', so using 'clang' instead"
    MY_CC="clang"
    ARGS="$ARGS -Wno-deprecated-declarations"
fi

if [[ $1 == "vector" ]]; then
    ARGS="$ARGS -D QUEUE_IMPL='V'"
    shift
fi

COMMAND="$MY_CC $ARGS $@"

printf "Running \"$COMMAND\"\n\n"

$COMMAND
