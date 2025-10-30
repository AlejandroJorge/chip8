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
    if(read(fd, memory+UPS, EOR - UPS) > 0) {
        perror("Error reading file");
        close(fd);
        return -1;
    }
    close(fd);
    return 1;
}

// Gets next opcode in memory and updates program counter
uint16_t get_next_opcode() {
    uint16_t opcode = memory[program_counter];
    program_counter += 2;
    return opcode;
}

// Parses opcode
enum Opcodes get_opcode_identifier(uint16_t opcode) {
    if(opcode == 0x00E0) return OPCODE_00E0;
    if(opcode == 0x00EE) return OPCODE_00EE;
    if((opcode & 0x1000) == 0x1000) return OPCODE_1NNN;
    if((opcode & 0x2000) == 0x2000) return OPCODE_2NNN;
    if((opcode & 0x3000) == 0x3000) return OPCODE_3XNN;
    if((opcode & 0x4000) == 0x4000) return OPCODE_4XNN;
    if((opcode & 0x5000) == 0x5000 && (opcode >> 12) == 0) return OPCODE_5XY0;
    if((opcode & 0x6000) == 0x6000) return OPCODE_6XNN;
    if((opcode & 0x7000) == 0x7000) return OPCODE_7XNN;
    if((opcode & 0x8000) == 0x8000) {
        uint8_t option = opcode >> 12;
        switch(option) {
            case 0x00:
                return OPCODE_8XY0;
            case 0x01:
                return OPCODE_8XY1;
            case 0x02:
                return OPCODE_8XY2;
            case 0x03:
                return OPCODE_8XY3;
            case 0x04:
                return OPCODE_8XY4;
            case 0x05:
                return OPCODE_8XY5;
            case 0x06:
                return OPCODE_8XY6;
            case 0x07:
                return OPCODE_8XY7;
            case 0x0E:
                return OPCODE_8XYE;
        }
    }
    if((opcode & 0x9000) == 0x9000 && (opcode >> 12) == 0) return OPCODE_9XY0;
    if((opcode & 0xA000) == 0xA000) return OPCODE_ANNN;
    if((opcode & 0xB000) == 0xB000) return OPCODE_BNNN;
    if((opcode & 0xC000) == 0xC000) return OPCODE_CXNN;
    if((opcode & 0xD000) == 0xD000) return OPCODE_DXYN;
    if((opcode & 0xE000) == 0xE000) {
        uint8_t option = opcode >> 8;
        if(option == 0x9E) return OPCODE_EX9E;
        if(option == 0xA1) return OPCODE_EXA1;
    }
    if((opcode & 0xF000) == 0xF000) {
        uint8_t option = opcode >> 8;
        switch(option) {
            case 0x07:
                return OPCODE_FX07;
            case 0x0A:
                return OPCODE_FX0A;
            case 0x15:
                return OPCODE_FX15;
            case 0x18:
                return OPCODE_FX18;
            case 0x1E:
                return OPCODE_FX1E;
            case 0x29:
                return OPCODE_FX29;
            case 0x33:
                return OPCODE_FX33;
            case 0x55:
                return OPCODE_FX55;
            case 0x65:
                return OPCODE_FX65;
        }
    }

    return -1;
}
