#!/bin/bash

gcc \
    -std=c90 \
    -g3 \
    -fsanitize=undefined \
    -fsanitize-trap \
    -Wall \
    -Wextra \
    -Wconversion \
    -Wno-sign-conversion \
    -Wdouble-promotion \
    "$1" \
    -o "./out/${1%.c}".out &&
    ./out/"${1%.c}".out "${@:2}"
