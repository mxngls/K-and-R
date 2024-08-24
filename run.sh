#!/bin/bash

gcc \
  -std=c90 \
  -fsanitize=undefined \
  -Wall \
  "$1" \
  -o "${1%.c}".out &&
  ./"${1%.c}".out "$*"
