#!/bin/bash

mkdir -p build
gcc ./src/*.c -Wall -lraylib -lm -Iinclude -g -fsanitize=address -o ./build/chip8
