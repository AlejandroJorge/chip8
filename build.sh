#!/bin/bash

mkdir -p build
gcc ./src/*.c -Wall -lraylib -Iinclude -g -fsanitize=address -o ./build/chip8
