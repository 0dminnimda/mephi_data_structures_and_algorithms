#!/usr/bin/env bash

INPUT="10 20 -2345 234 0"

COMMAND="bash build.sh $1 -D TEST -Wno-unused-result"

$COMMAND &> /dev/null || exit 1
./main.out <<< "$INPUT $INPUT"
echo "------------------------------------------------------------"

$COMMAND &> /dev/null || exit 1
./main.out <<< $INPUT
echo "------------------------------------------------------------"

$COMMAND -D FAILURE_TEST=1 &> /dev/null || exit 1
./main.out
echo "------------------------------------------------------------"

$COMMAND -D FAILURE_TEST=1 &> /dev/null || exit 1
./main.out
echo "------------------------------------------------------------"

$COMMAND -D FAILURE_TEST=1 &> /dev/null || exit 1
./main.out
echo "------------------------------------------------------------"

if [[ $1 != "vector" ]]; then
    $0 vector
fi
