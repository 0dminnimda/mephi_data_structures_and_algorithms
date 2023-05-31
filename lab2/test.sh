#!/usr/bin/env bash

INPUT="10 20 -2345 234 0"

BAD_COMP="echo Bad compilation; exit -1"
BAD_RET="echo Bad return code; exit 1"
BUILD="bash build.sh $1 -D TEST -Wno-unused-result"
RUN="./main.out"

$BUILD &> /dev/null || $BAD_COMP
$RUN <<< "$INPUT $INPUT" || $BAD_RET
echo "------------------------------------------------------------"

$BUILD &> /dev/null || $BAD_COMP
$RUN <<< $INPUT || $BAD_RET
echo "------------------------------------------------------------"

$BUILD -D FAILURE_TEST=1 &> /dev/null || $BAD_COMP
$RUN
echo "------------------------------------------------------------"

$BUILD -D FAILURE_TEST=2 &> /dev/null || $BAD_COMP
$RUN
echo "------------------------------------------------------------"

$BUILD -D FAILURE_TEST=3 &> /dev/null || $BAD_COMP
$RUN
echo "------------------------------------------------------------"

if [[ $1 != "vector" ]]; then
    $0 vector
fi
