#include "state.h"
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <sys/types.h>

// CLS — 00E0
// Clears the display buffer.
void opcode_00e0_handler(uint16_t opcode) {
  for (int i = 0; i < SW * SH; i++) {
    screen[i] = false;
  }
}

// RET — 00EE
// Restores the return address from the stack.
void opcode_00ee_handler(uint16_t opcode) {
  program_counter = stack[stack_pointer];
  stack_pointer -= 2;
}

// JMP — 1NNN
// Jumps directly to address NNN.
void opcode_1nnn_handler(uint16_t opcode) {
  uint16_t n = opcode & 0x0FFF;
  program_counter = n;
}

// CALL NNN — 2NNN
// Jumps to subroutine and stores the return address.
void opcode_2nnn_handler(uint16_t opcode) {
  uint16_t n = opcode & 0x0FFF;
  stack_pointer += 2;
  stack[stack_pointer] = program_counter;
  program_counter = n;
}

// SE VX, NN — 3XNN
// Skips next instruction when register equals NN.
void opcode_3xnn_handler(uint16_t opcode) {
  uint8_t n_register = opcode & 0x0F00 >> 2, n = opcode & 0x00FF;
  if (registers[n_register] == n)
    program_counter += 2;
}

// SNE VX, NN — 4XNN
// Skips next instruction when register differs from NN.
void opcode_4xnn_handler(uint16_t opcode) {
  uint8_t n_register = opcode & 0x0F00 >> 2, n = opcode & 0x00FF;
  if (registers[n_register] != n)
    program_counter += 2;
}

// SE VX, VY — 5XY0
// Skips next instruction when registers are equal.
void opcode_5xy0_handler(uint16_t opcode) {
  uint8_t n1_register = opcode & 0x0F00 >> 2,
          n2_register = opcode & 0x00F0 >> 1;
  if (registers[n1_register] == registers[n2_register])
    program_counter += 2;
}

// LD VX, NN — 6XNN
// Loads immediate NN into register VX.
void opcode_6xnn_handler(uint16_t opcode) {
  uint8_t n_register = opcode & 0x0F00 >> 2, n = opcode & 0x00FF;
  registers[n_register] = n;
}

// ADD VX, NN — 7XNN
// Adds immediate NN to register VX.
void opcode_7xnn_handler(uint16_t opcode) {
  uint8_t n_register = opcode & 0x0F00 >> 2, n = opcode & 0x00FF;
  registers[n_register] += n;
}

// LD VX, VY — 8XY0
// Copies VY into VX.
void opcode_8xy0_handler(uint16_t opcode) {
  uint8_t n1_register = opcode & 0x0F00 >> 2,
          n2_register = opcode & 0x00F0 >> 1;
  registers[n1_register] = registers[n2_register];
}

// OR VX, VY — 8XY1
// Performs bitwise OR between VX and VY.
void opcode_8xy1_handler(uint16_t opcode) {
  uint8_t n1_register = opcode & 0x0F00 >> 2,
          n2_register = opcode & 0x00F0 >> 1;
  registers[n1_register] = registers[n1_register] | registers[n2_register];
}

// AND VX, VY — 8XY2
// Performs bitwise AND between VX and VY.
void opcode_8xy2_handler(uint16_t opcode) {
  uint8_t n1_register = opcode & 0x0F00 >> 2,
          n2_register = opcode & 0x00F0 >> 1;
  registers[n1_register] = registers[n1_register] & registers[n2_register];
}

// XOR VX, VY — 8XY3
// Performs bitwise XOR between VX and VY.
void opcode_8xy3_handler(uint16_t opcode) {
  uint8_t n1_register = opcode & 0x0F00 >> 2,
          n2_register = opcode & 0x00F0 >> 1;
  registers[n1_register] = registers[n1_register] ^ registers[n2_register];
}

// ADD VX, VY — 8XY4
// Adds VY to VX and sets VF on overflow.
void opcode_8xy4_handler(uint16_t opcode) {
  uint8_t n1_register = opcode & 0x0F00 >> 2,
          n2_register = opcode & 0x00F0 >> 1;
  if (registers[n1_register] + registers[n2_register] > OVF) {
    VF = 1;
  } else {
    VF = 0;
  }
  registers[n1_register] += registers[n2_register];
}

// SUB VX, VY — 8XY5
// Subtracts VY from VX and sets VF when borrow does not occur.
void opcode_8xy5_handler(uint16_t opcode) {
  uint8_t n1_register = opcode & 0x0F00 >> 2,
          n2_register = opcode & 0x00F0 >> 1;
  if (registers[n1_register] > registers[n2_register]) {
    VF = 1;
  } else {
    VF = 0;
  }
  registers[n1_register] -= registers[n2_register];
}

// SHR VX {, VY} — 8XY6
// Shifts VX right and moves LSB into VF.
void opcode_8xy6_handler(uint16_t opcode) {
  uint8_t n1_register = opcode & 0x0F00 >> 2,
          n2_register = opcode & 0x00F0 >> 1;
  VF = registers[n1_register] & 0x01;
  registers[n1_register] = registers[n1_register] >> 1;
}

// SUBN VX, VY — 8XY7
// Sets VX to VY minus VX and updates VF for borrow.
void opcode_8xy7_handler(uint16_t opcode) {
  uint8_t n1_register = opcode & 0x0F00 >> 2,
          n2_register = opcode & 0x00F0 >> 1;
  if (registers[n2_register] > registers[n1_register]) {
    VF = 1;
  } else {
    VF = 0;
  }
  registers[n2_register] -= registers[n1_register];
}

// SHL VX {, VY} — 8XYE
// Shifts VX left and moves MSB into VF.
void opcode_8xye_handler(uint16_t opcode) {
  uint8_t n1_register = opcode & 0x0F00 >> 2,
          n2_register = opcode & 0x00F0 >> 1;
  VF = registers[n1_register] & 0x80;
  registers[n1_register] = registers[n1_register] << 1;
}

// SNE VX, VY — 9XY0
// Skips next instruction when registers differ.
void opcode_9xy0_handler(uint16_t opcode) {
  uint8_t n1_register = opcode & 0x0F00 >> 2,
          n2_register = opcode & 0x00F0 >> 1;
  if (registers[n1_register] != registers[n2_register])
    program_counter += 2;
}

// LD I, NNN — ANNN
// Loads address NNN into the index register.
void opcode_annn_handler(uint16_t opcode) {
  uint16_t n = opcode & 0x0FFF;
  index_register = n;
}

// JMP V0, NNN — BNNN
// Jumps to address offset by V0.
void opcode_bnnn_handler(uint16_t opcode) {
  uint16_t nnn = opcode & 0x0FFF;
  program_counter = registers[0] + nnn;
}

// RND VX, NN – CXNN
// Stores random value masked by NN into VX.
void opcode_cxnn_handler(uint16_t opcode) {
  uint8_t reg = opcode & 0x0F00 >> 2, nn = opcode & 0x00FF;
  uint8_t random_n = rand() & 0xFF; // Parse into uint8_n
  registers[reg] = random_n & nn;
}

// DRW VX, VY, N — DXYN
// Draws N-byte sprite at coordinates in VX and VY.
void opcode_dxyn_handler(uint16_t opcode) {
  uint8_t reg_x = opcode & 0x0F00 >> 2, reg_y = opcode & 0x00F0 >> 1,
          n = opcode & 0x000F;
  uint8_t x = registers[reg_x] % SW;
  uint8_t y = registers[reg_y] % SH;
  // TODO: Finish this implementation
}

// SKP VX — EX9E
// Skips next instruction when key in VX is pressed.
void opcode_ex9e_handler(uint16_t opcode) {
  uint8_t reg = opcode & 0x0F00 >> 2;
  if (keys_pressed[reg])
    program_counter += 2;
}

// SKNP VX — EXA1
// Skips next instruction when key in VX is not pressed.
void opcode_exa1_handler(uint16_t opcode) {
  uint8_t reg = opcode & 0x0F00 >> 2;
  if (!keys_pressed[reg])
    program_counter += 2;
}

// LD VX, DT — FX07
// Loads delay timer into VX.
void opcode_fx07_handler(uint16_t opcode) { 
  uint8_t reg = opcode & 0x0F00 >> 2;
  registers[reg] = delay_register; 
}

// LD VX, K — FX0A
// Blocks until a key is pressed and stores key index.
void opcode_fx0a_handler(uint16_t opcode) {
  uint8_t reg = opcode & 0x0F00 >> 2;
  if (keys_pressed[0])
    registers[reg] = 0;
  else if (keys_pressed[1])
    registers[reg] = 1;
  else if (keys_pressed[2])
    registers[reg] = 2;
  else if (keys_pressed[3])
    registers[reg] = 3;
  else if (keys_pressed[4])
    registers[reg] = 4;
  else if (keys_pressed[5])
    registers[reg] = 5;
  else if (keys_pressed[6])
    registers[reg] = 6;
  else if (keys_pressed[7])
    registers[reg] = 7;
  else if (keys_pressed[8])
    registers[reg] = 8;
  else if (keys_pressed[9])
    registers[reg] = 9;
  else if (keys_pressed[10])
    registers[reg] = 10;
  else if (keys_pressed[11])
    registers[reg] = 11;
  else if (keys_pressed[12])
    registers[reg] = 12;
  else if (keys_pressed[13])
    registers[reg] = 13;
  else if (keys_pressed[14])
    registers[reg] = 14;
  else if (keys_pressed[15])
    registers[reg] = 15;
  else
    program_counter -= 2;
}

// LD DT, VX — FX15
// Sets the delay timer from VX.
void opcode_fx15_handler(uint16_t opcode) { 
  uint8_t reg = opcode & 0x0F00 >> 2;
  delay_register = registers[reg]; 
}

// LD ST, VX — FX18
// Sets the sound timer from VX.
void opcode_fx18_handler(uint16_t opcode) { 
  uint8_t reg = opcode & 0x0F00 >> 2;
  sound_register = registers[reg]; 
}

// ADD I, VX — FX1E
// Adds VX to the index register.
void opcode_fx1e_handler(uint16_t opcode) {
  uint8_t reg = opcode & 0x0F00 >> 2;
  index_register = index_register + registers[reg];
}

// LD F, VX — FX29
// Points I to the sprite for digit in VX.
void opcode_fx29_handler(uint16_t opcode) {
  uint8_t reg = opcode & 0x0F00 >> 2;
  index_register = registers[reg] * 0x05;
}

// LD B, VX — FX33
// Stores BCD representation of VX at I, I+1, I+2.
void opcode_fx33_handler(uint16_t opcode) {
  uint8_t reg = opcode & 0x0F00 >> 2;
  uint8_t hundreds = registers[reg] / 100;
  uint8_t tens = (registers[reg] - hundreds * 100) / 10;
  uint8_t units = registers[reg] - hundreds * 100 - tens * 10;
  memory[index_register] = hundreds;
  memory[index_register + 1] = tens;
  memory[index_register + 2] = registers[reg];
}

// LD [I], VX — FX55
// Stores registers V0 through VX into memory.
void opcode_fx55_handler(uint16_t opcode) {
  uint8_t reg = opcode & 0x0F00 >> 2;
  reg = reg & 0x0F;
  for (uint8_t i = 0; i < reg; i++)
    memory[index_register + i] = registers[reg + i];
}

// LD VX, [I] — FX65
// Loads registers V0 through VX from memory.
void opcode_fx65_handler(uint16_t opcode) {
  uint8_t reg = opcode & 0x0F00 >> 2;
  for (uint8_t i = 0; i < reg; i++)
    registers[reg + i] = memory[index_register + i];
}
