#!/bin/bash

gcc $(find ./src -name "*.c" -not -name "main.c") ./test/*.c -o ./build/test
./build/test
rm ./build/test
