#!/bin/bash

mkdir -p build
gcc $(find ./src -name "*.c" -not -name "main.c") ./test/*.c -Iinclude -lraylib -lm -o ./build/test
./build/test
rm ./build/test
