#!/bin/bash

gcc \
    -xc \
    -std=c90 \
    -fsanitize=undefined \
    -Wall \
    -Wextra \
    -Wconversion \
    -Wsign-conversion \
    -Wdouble-promotion \
    -Werror \
    -Wpedantic \
    -Wpointer-arith \
    -D_FORTIFY_SOURCE=2 \
    "$1" \
    -o "./out/${1%.c}".out
