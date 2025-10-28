#ifndef STATE_H
#define STATE_H

#include <stdint.h>

#define UPS 0x600 // User Program Start, ETI 660
#define EOR 0xFFF // End of RAM
#define MSA 0xFDF // Minimum Stack Address (EOR - 2 * 0x10) given it's 16
#define OVF 0xFF  // Overflow for arithmetic operations
#define SH 32     // Screen Height in pixels
#define SW 64     // Screen Width in pixels

// TODO: Do some of these require to be lock guarded? A separate thread to handle I/O?
uint8_t memory[EOR] = {0};
uint8_t registers[16] = {0};
uint16_t program_counter = 0;
uint16_t index_register = 0;
uint16_t stack[16] = {0};
uint8_t stack_pointer = 0;
uint8_t sound_register = 0;
uint8_t delay_register = 0;
bool screen[SW * SH] = {0};
bool keys_pressed[16] = {0};

#define VF registers[0xF]

#endif
