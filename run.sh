#!/bin/bash

gcc \
  -std=c90 \
  -fsanitize=undefined \
  -Wall \
  "$1" \
  -o "./out/${1%.c}".out &&
  ./out/"${1%.c}".out "$*"
