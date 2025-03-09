#!/bin/bash

set -e

OUT_DIR="out"

# Compile both versions using run.sh
echo "Compiling programs..."
../compile.sh ./01_cat.c
../compile.sh ./01_cat_std.c

# Create a large test file
echo "Creating test file (this may take a moment)..."
dd if=/dev/zero of="$OUT_DIR/testfile.txt" count=1024 bs=262144

# Warming up (to ensure file is in cache)
echo "Warming up..."
cat "$OUT_DIR/testfile.txt" >/dev/null
cat "$OUT_DIR/testfile.txt" >/dev/null
cat "$OUT_DIR/testfile.txt" >/dev/null

echo ""
echo "System cat command:"
time cat "$OUT_DIR/testfile.txt" >/dev/null
time cat "$OUT_DIR/testfile.txt" >/dev/null
time cat "$OUT_DIR/testfile.txt" >/dev/null

echo ""
echo "Using system calls version:"
time "$OUT_DIR/01_cat.out" "$OUT_DIR/testfile.txt" >/dev/null
time "$OUT_DIR/01_cat.out" "$OUT_DIR/testfile.txt" >/dev/null
time "$OUT_DIR/01_cat.out" "$OUT_DIR/testfile.txt" >/dev/null

echo ""
echo "Using standard library version:"
time "$OUT_DIR/01_cat_std.out" "$OUT_DIR/testfile.txt" >/dev/null
time "$OUT_DIR/01_cat_std.out" "$OUT_DIR/testfile.txt" >/dev/null
time "$OUT_DIR/01_cat_std.out" "$OUT_DIR/testfile.txt" >/dev/null

# Clean up
echo ""
echo "Cleaning up..."
rm "$OUT_DIR/testfile.txt"
