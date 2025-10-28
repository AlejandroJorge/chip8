#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

#define UPS 0x600 // User Program Start, ETI 660
#define EOR 0xFFF // End of RAM
#define MSA 0xFDF // Minimum Stack Address (EOR - 2 * 0x10) given it's 16
#define OVF 0xFF  // Overflow for arithmetic operations
#define SH 32     // Screen Height in pixels
#define SW 64     // Screen Width in pixels

// Do these require to be lock guarded?
// [IDEA] Separate thread for handling I/O
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

// TODO: Change function names to: opcode_xxxx_handler()
// Include a brief summary as a comment

// CLS — 00E0
void opcode_clear() {
  for (int i = 0; i < SW * SH; i++) {
    screen[i] = false;
  }
}

// RET — 00EE
void opcode_return() {
  program_counter = stack[stack_pointer];
  stack_pointer -= 2;
}

// JMP — 1NNN
void opcode_jump(uint16_t n) { program_counter = n; }

// CALL NNN — 2NNN
void opcode_call(uint16_t n) {
  stack_pointer += 2;
  stack[stack_pointer] = program_counter;
  program_counter = n;
}

// SE VX, NN — 3XNN
void opcode_skip_equal(uint8_t n_register, uint8_t n) {
  if (registers[n_register] == n)
    program_counter += 2;
}

// SNE VX, NN — 4XNN
void opcode_skip_not_equal_value(uint8_t n_register, uint8_t n) {
  if (registers[n_register] != n)
    program_counter += 2;
}

// SE VX, VY — 5XY0
void opcode_skip_equal_register(uint8_t n1_register, uint8_t n2_register) {
  if (registers[n1_register] == registers[n2_register])
    program_counter += 2;
}

// LD VX, NN — 6XNN
void opcode_load_value(uint8_t n_register, uint8_t n) {
  registers[n_register] = n;
}

// ADD VX, NN — 7XNN
void opcode_add_value(uint8_t n_register, uint8_t n) {
  registers[n_register] += n;
}

// LD VX, VY — 8XY0
void opcode_load_register(uint8_t n1_register, uint8_t n2_register) {
  registers[n1_register] = registers[n2_register];
}

// OR VX, VY — 8XY1
void opcode_or(uint8_t n1_register, uint8_t n2_register) {
  registers[n1_register] = registers[n1_register] | registers[n2_register];
}

// AND VX, VY — 8XY2
void opcode_and(uint8_t n1_register, uint8_t n2_register) {
  registers[n1_register] = registers[n1_register] & registers[n2_register];
}

// XOR VX, VY — 8XY3
void opcode_xor(uint8_t n1_register, uint8_t n2_register) {
  registers[n1_register] = registers[n1_register] ^ registers[n2_register];
}

// ADD VX, VY — 8XY4
void opcode_add_register(uint8_t n1_register, uint8_t n2_register) {
  if (registers[n1_register] + registers[n2_register] > OVF) {
    VF = 1;
  } else {
    VF = 0;
  }
  registers[n1_register] += registers[n2_register];
}

// SUB VX, VY — 8XY5
void opcode_sub(uint8_t n1_register, uint8_t n2_register) {
  if (registers[n1_register] > registers[n2_register]) {
    VF = 1;
  } else {
    VF = 0;
  }
  registers[n1_register] -= registers[n2_register];
}

// SHR VX {, VY} — 8XY6
void opcode_shiftr(uint8_t n1_register, uint8_t n2_register) {
  VF = registers[n1_register] & 0x01;
  registers[n1_register] = registers[n1_register] >> 1;
}

// SUBN VX, VY — 8XY7
void opcode_subn(uint8_t n1_register, uint8_t n2_register) {
  if (registers[n2_register] > registers[n1_register]) {
    VF = 1;
  } else {
    VF = 0;
  }
  registers[n2_register] -= registers[n1_register];
}

// SHL VX {, VY} — 8XYE
void opcode_shiftl(uint8_t n1_register, uint8_t n2_register) {
  VF = registers[n1_register] & 0x80;
  registers[n1_register] = registers[n1_register] << 1;
}

// SNE VX, VY — 9XY0
void opcode_skip_not_equal_register(uint8_t n1_register, uint8_t n2_register) {
  if (registers[n1_register] != registers[n2_register])
    program_counter += 2;
}

// LD I, NNN — ANNN
void opcode_load_index(uint16_t n) { index_register = n; }

// JMP V0, NNN — BNNN
void opcode_jump_offset(uint32_t nnn) {
  nnn = nnn & 0x0FFF;
  program_counter = registers[0] + nnn;
}

// RND VX, NN – CXNN
void opcode_random(uint8_t reg, uint16_t nn) {
  uint8_t random_n = rand() & 0xFF; // Parse into uint8_n
  registers[reg] = random_n & nn;
}

// DRW VX, VY, N — DXYN
void opcode_draw(uint8_t reg_x, uint8_t reg_y, uint8_t n) {
  uint8_t x = registers[reg_x] % SW;
  uint8_t y = registers[reg_y] % SH;
  // TODO: Finish this implementation
}

// SKP VX — EX9E
void opcode_skip_when_key_pressed(uint8_t reg) {
  if (keys_pressed[reg])
    program_counter += 2;
}

// SKNP VX — EXA1
void opcode_skip_when_not_key_pressed(uint8_t reg) {
  if (keys_pressed[reg])
    program_counter += 2;
}

// LD VX, DT — FX07
void opcode_read_delay(uint8_t reg) { registers[reg] = delay_register; }

// LD VX, K — FX0A
void opcode_wait_for_keypress(uint8_t reg) {
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
void opcode_load_delay(uint8_t reg) { delay_register = registers[reg]; }

// LD ST, VX — FX18
void opcode_load_sound(uint8_t reg) { sound_register = registers[reg]; }

// ADD I, VX — FX1E
void opcode_add_index_register(uint8_t reg) {
  index_register = index_register + registers[reg];
}

// LD F, VX — FX29
void opcode_set_digit_sprite(uint8_t reg) {
  index_register = registers[reg] * 0x05;
}

// LD B, VX — FX33
void opcode_store_decimal(uint8_t reg) {
  uint8_t hundreds = registers[reg] / 100;
  uint8_t tens = (registers[reg] - hundreds * 100) / 10;
  uint8_t units = registers[reg] - hundreds * 100 - tens * 10;
  memory[index_register] = hundreds;
  memory[index_register + 1] = tens;
  memory[index_register + 2] = registers[reg];
}

// LD [I], VX — FX55
void opcode_store_memory_chunk(uint8_t reg) {
  reg = reg & 0x0F;
  for (uint8_t i = 0; i < reg; i++)
    memory[index_register + i] = registers[reg + i];
}

// LD VX, [I] — FX65
void opcode_load_memory_chunk(uint8_t reg) {
  reg = reg & 0x0F;
  for (uint8_t i = 0; i < reg; i++)
    registers[reg + i] = memory[index_register + i];
}

void parser(uint16_t opcode) {
  // TODO: Implement whole parser
}

int main() {

  srand(0);

  return 0;
}
