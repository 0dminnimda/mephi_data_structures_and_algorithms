#!/usr/bin/env bash

MY_CC="cc"
ARGS="main.c -o main.out -O3 -g"

which cc &> /dev/null

if [[ $? != "0" ]]; then
    echo "I did not find 'cc', so using 'clang' instead"
    MY_CC="clang"
    ARGS="$ARGS -Wno-deprecated-declarations"
fi

COMMAND="$MY_CC $ARGS"

printf "Running '$COMMAND'\n\n"

$COMMAND
