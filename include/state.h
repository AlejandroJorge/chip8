#pragma once

#include <stdint.h>
#include <stdbool.h>

#define UPS 0x200 // User Program Start, ETI 660
#define EOR 0xFFF // End of RAM
#define MSA 0xFDF // Minimum Stack Address (EOR - 2 * 0x10) given it's 16
#define OVF 0xFF  // Overflow for arithmetic operations
#define SH 32     // Screen Height in pixels
#define SW 64     // Screen Width in pixels

// TODO: A separate thread to handle I/O
extern uint8_t memory[EOR];
extern uint8_t registers[16];
extern uint16_t program_counter;
extern uint16_t index_register;
extern uint16_t stack[16];
extern uint8_t stack_pointer;
extern uint8_t sound_register;
extern uint8_t delay_register;
extern bool screen[SW * SH];
extern bool keys_pressed[16];

#define VF registers[0xF]
