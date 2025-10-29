#include <stdint.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>

#include "opcodes.h"

#define PENE 32

int source_fd = 0; // Global state (but private for other files)
uint16_t opcode_buffer[PENE] = {0};

// TODO: Open file and set source_fd on this file
// remember to set -1 on error
int init_parser(char *filename) {
    if ((source_fd = open(filename, O_RDONLY)) == -1) {
        perror("Error opening file");
        return -1;
    }
    return 1;
}

// TODO: Implement, can be just a read syscall, should be chunked tho
// returns 0 on end of stream
uint16_t get_next_opcode() {
    if(*opcode_buffer              ) {
        read(source_fd, opcode_buffer, sizeof(uint16_t)*PENE);
    }


}

// TODO: Implement actual parsing, 0x1F3C -> opcode_type
enum Opcodes get_opcode_identifier(uint16_t opcode) {
    return OPCODE_00E0;
}
