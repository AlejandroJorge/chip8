#include "state.h"

uint8_t memory[EOR] = {0};
uint8_t registers[16] = {0};
uint16_t program_counter = UPS;
uint16_t index_register = 0;
uint16_t stack[16] = {0};
uint8_t stack_pointer = 0;
uint8_t sound_register = 0;
uint8_t delay_register = 0;
bool screen[SW * SH] = {0};
bool keys_pressed[16] = {0};
