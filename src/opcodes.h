#ifndef OPCODES_H
#define OPCODES_H

#include <stdint.h>

enum Opcodes {
  OPCODE_00E0,
  OPCODE_00EE,
  OPCODE_1NNN,
  OPCODE_2NNN,
  OPCODE_3XNN,
  OPCODE_4XNN,
  OPCODE_5XY0,
  OPCODE_6XNN,
  OPCODE_7XNN,
  OPCODE_8XY0,
  OPCODE_8XY1,
  OPCODE_8XY2,
  OPCODE_8XY3,
  OPCODE_8XY4,
  OPCODE_8XY5,
  OPCODE_8XY6,
  OPCODE_8XY7,
  OPCODE_8XYE,
  OPCODE_9XY0,
  OPCODE_ANNN,
  OPCODE_BNNN,
  OPCODE_CXNN,
  OPCODE_DXYN,
  OPCODE_EX9E,
  OPCODE_EXA1,
  OPCODE_FX07,
  OPCODE_FX0A,
  OPCODE_FX15,
  OPCODE_FX18,
  OPCODE_FX1E,
  OPCODE_FX29,
  OPCODE_FX33,
  OPCODE_FX55,
  OPCODE_FX65
};

// CLS — 00E0
// Clears the display buffer.
void opcode_00e0_handler(void);

// RET — 00EE
// Restores the return address from the stack.
void opcode_00ee_handler(void);

// JMP — 1NNN
// Jumps directly to address NNN.
void opcode_1nnn_handler(uint16_t n);

// CALL NNN — 2NNN
// Jumps to subroutine and stores the return address.
void opcode_2nnn_handler(uint16_t n);

// SE VX, NN — 3XNN
// Skips next instruction when register equals NN.
void opcode_3xnn_handler(uint8_t n_register, uint8_t n);

// SNE VX, NN — 4XNN
// Skips next instruction when register differs from NN.
void opcode_4xnn_handler(uint8_t n_register, uint8_t n);

// SE VX, VY — 5XY0
// Skips next instruction when registers are equal.
void opcode_5xy0_handler(uint8_t n1_register, uint8_t n2_register);

// LD VX, NN — 6XNN
// Loads immediate NN into register VX.
void opcode_6xnn_handler(uint8_t n_register, uint8_t n);

// ADD VX, NN — 7XNN
// Adds immediate NN to register VX.
void opcode_7xnn_handler(uint8_t n_register, uint8_t n);

// LD VX, VY — 8XY0
// Copies VY into VX.
void opcode_8xy0_handler(uint8_t n1_register, uint8_t n2_register);

// OR VX, VY — 8XY1
// Performs bitwise OR between VX and VY.
void opcode_8xy1_handler(uint8_t n1_register, uint8_t n2_register);

// AND VX, VY — 8XY2
// Performs bitwise AND between VX and VY.
void opcode_8xy2_handler(uint8_t n1_register, uint8_t n2_register);

// XOR VX, VY — 8XY3
// Performs bitwise XOR between VX and VY.
void opcode_8xy3_handler(uint8_t n1_register, uint8_t n2_register);

// ADD VX, VY — 8XY4
// Adds VY to VX and sets VF on overflow.
void opcode_8xy4_handler(uint8_t n1_register, uint8_t n2_register);

// SUB VX, VY — 8XY5
// Subtracts VY from VX and sets VF when borrow does not occur.
void opcode_8xy5_handler(uint8_t n1_register, uint8_t n2_register);

// SHR VX {, VY} — 8XY6
// Shifts VX right and moves LSB into VF.
void opcode_8xy6_handler(uint8_t n1_register, uint8_t n2_register);

// SUBN VX, VY — 8XY7
// Sets VX to VY minus VX and updates VF for borrow.
void opcode_8xy7_handler(uint8_t n1_register, uint8_t n2_register);

// SHL VX {, VY} — 8XYE
// Shifts VX left and moves MSB into VF.
void opcode_8xye_handler(uint8_t n1_register, uint8_t n2_register);

// SNE VX, VY — 9XY0
// Skips next instruction when registers differ.
void opcode_9xy0_handler(uint8_t n1_register, uint8_t n2_register);

// LD I, NNN — ANNN
// Loads address NNN into the index register.
void opcode_annn_handler(uint16_t n);

// JMP V0, NNN — BNNN
// Jumps to address offset by V0.
void opcode_bnnn_handler(uint32_t nnn);

// RND VX, NN – CXNN
// Stores random value masked by NN into VX.
void opcode_cxnn_handler(uint8_t reg, uint16_t nn);

// DRW VX, VY, N — DXYN
// Draws N-byte sprite at coordinates in VX and VY.
void opcode_dxyn_handler(uint8_t reg_x, uint8_t reg_y, uint8_t n);

// SKP VX — EX9E
// Skips next instruction when key in VX is pressed.
void opcode_ex9e_handler(uint8_t reg);

// SKNP VX — EXA1
// Skips next instruction when key in VX is not pressed.
void opcode_exa1_handler(uint8_t reg);

// LD VX, DT — FX07
// Loads delay timer into VX.
void opcode_fx07_handler(uint8_t reg);

// LD VX, K — FX0A
// Blocks until a key is pressed and stores key index.
void opcode_fx0a_handler(uint8_t reg);

// LD DT, VX — FX15
// Sets the delay timer from VX.
void opcode_fx15_handler(uint8_t reg);

// LD ST, VX — FX18
// Sets the sound timer from VX.
void opcode_fx18_handler(uint8_t reg);

// ADD I, VX — FX1E
// Adds VX to the index register.
void opcode_fx1e_handler(uint8_t reg);

// LD F, VX — FX29
// Points I to the sprite for digit in VX.
void opcode_fx29_handler(uint8_t reg);

// LD B, VX — FX33
// Stores BCD representation of VX at I, I+1, I+2.
void opcode_fx33_handler(uint8_t reg);

// LD [I], VX — FX55
// Stores registers V0 through VX into memory.
void opcode_fx55_handler(uint8_t reg);

// LD VX, [I] — FX65
// Loads registers V0 through VX from memory.
void opcode_fx65_handler(uint8_t reg);

#endif
