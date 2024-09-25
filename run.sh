#!/bin/bash

gcc \
    -std=c90 \
    -fsanitize=undefined \
    -Wall \
    -Wextra \
    -Wconversion \
    -Wno-sign-conversion \
    -Wdouble-promotion \
    "$1" \
    -o "./out/${1%.c}".out &&
    ./out/"${1%.c}".out "${@:2}"
