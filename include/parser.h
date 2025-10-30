#pragma once

#include <stdint.h>
#include <stdint.h>
#include "opcodes.h"

int init_parser(char* filename);

uint16_t get_next_opcode();

enum Opcodes get_opcode_identifier(uint16_t opcode);
