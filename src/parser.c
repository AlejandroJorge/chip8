#include <stdint.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>

#include "opcodes.h"
#include "state.h"

// Reads ROM and loads it into user memory space
int load_rom(char *filename) {
    int fd;
    if ((fd = open(filename, O_RDONLY)) == -1) {
        perror("Error opening file");
        return -1;
    }
    if(read(fd, memory+UPS, EOR - UPS) == -1) {
        perror("Error reading file");
        close(fd);
        return -1;
    }
    close(fd);
    return 1;
}

// Gets next opcode in memory and updates program counter
uint16_t get_next_opcode() {
    uint16_t opcode = (memory[program_counter] << 8) | memory[program_counter+1];
    program_counter += 2;
    return opcode;
}

// Parses opcode
enum Opcodes get_opcode_identifier(uint16_t opcode) {
    switch(opcode & 0xF000) {
        case 0x0000:
            if(opcode == 0x00E0) return OPCODE_00E0;
            if(opcode == 0x00EE) return OPCODE_00EE;
            break;
        case 0x1000: return OPCODE_1NNN;
        case 0x2000: return OPCODE_2NNN;
        case 0x3000: return OPCODE_3XNN;
        case 0x4000: return OPCODE_4XNN;
        case 0x5000:
            if((opcode & 0x000F) == 0x0000) return OPCODE_5XY0;
            break;
        case 0x6000: return OPCODE_6XNN;
        case 0x7000: return OPCODE_7XNN;
        case 0x8000:
            switch(opcode & 0x000F) {
                case 0x0000: return OPCODE_8XY0;
                case 0x0001: return OPCODE_8XY1;
                case 0x0002: return OPCODE_8XY2;
                case 0x0003: return OPCODE_8XY3;
                case 0x0004: return OPCODE_8XY4;
                case 0x0005: return OPCODE_8XY5;
                case 0x0006: return OPCODE_8XY6;
                case 0x0007: return OPCODE_8XY7;
                case 0x000E: return OPCODE_8XYE;
            }
            break;
        case 0x9000:
            if((opcode & 0x000F) == 0x0000) return OPCODE_9XY0;
            break;
        case 0xA000: return OPCODE_ANNN;
        case 0xB000: return OPCODE_BNNN;
        case 0xC000: return OPCODE_CXNN;
        case 0xD000: return OPCODE_DXYN;
        case 0xE000:
            switch(opcode & 0x00FF) {
                case 0x009E: return OPCODE_EX9E;
                case 0x00A1: return OPCODE_EXA1;
            }
            break;
        case 0xF000:
            switch(opcode & 0x00FF) {
                case 0x0007: return OPCODE_FX07;
                case 0x000A: return OPCODE_FX0A;
                case 0x0015: return OPCODE_FX15;
                case 0x0018: return OPCODE_FX18;
                case 0x001E: return OPCODE_FX1E;
                case 0x0029: return OPCODE_FX29;
                case 0x0033: return OPCODE_FX33;
                case 0x0055: return OPCODE_FX55;
                case 0x0065: return OPCODE_FX65;
            }
            break;
    }

    return -1;
}
