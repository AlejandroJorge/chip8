#!/bin/bash

mkdir -p build
gcc ./src/*.c -Wall -lraylib -Iinclude -o ./build/chip8
