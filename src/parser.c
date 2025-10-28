#include <stdint.h>

#include "opcodes.h"

int source_fd = 0; // Global state (but private for other files)

// TODO: Open file and set source_fd on this file
// remember to set -1 on error
int init_parser(char *filename) { return -1; }

// TODO: Implement, can be just a read syscall, should be chunked tho
// returns 0 on end of stream
uint16_t get_next_opcode() { return 0x0000; }

// TODO: Implement actual parsing, 0x1F3C -> opcode_type
enum Opcodes get_opcode_identifier(uint16_t opcode) {return OPCODE_00E0;}
