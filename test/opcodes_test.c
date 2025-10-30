#include "opcodes_test.h"
#include "opcodes.h"
#include "state.h"
#include "test_utils.h"
#include <stdatomic.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <sys/types.h>

INIT_TEST_MODULE(opcode);

bool test_00E0_clears_full_screen() {
  for (int i = 0; i < SW * SH; i++)
    screen[i] = true;

  opcode_00e0_handler(0x0000);

  bool is_screen_clear = true;
  for (int i = 0; i < SW * SH; i++)
    is_screen_clear &= !screen[i];

  ASSERT(is_screen_clear);
  return true;
}

bool test_00EE_restores_address() {
  program_counter = 0x02AF;
  stack_pointer = 4;
  stack[stack_pointer] = 0x0120;

  opcode_00ee_handler(0x00EE);
  ASSERT(stack_pointer == 2);
  ASSERT(program_counter == 0x0120);

  return true;
}

bool test_1NNN_moves_pc_to_inmediate() {
  program_counter = 0x05FD;
  uint16_t address = 0x0100;

  opcode_1nnn_handler(0x1100);

  ASSERT(program_counter == address);
  return true;
}

bool test_2NNN_moves_pc_and_stores_inmediate() {
  stack_pointer = 2;
  program_counter = 0x0542;

  opcode_2nnn_handler(0x2124);
  ASSERT(stack_pointer == 4);
  ASSERT(stack[stack_pointer] == 0x0542);
  ASSERT(program_counter == 0x0124);
  return true;
}

bool test_3XNN_skips_when_equal() {
  program_counter = 0x0100;
  registers[0x0A] = 0x42;

  opcode_3xnn_handler(0x3A42);

  ASSERT(program_counter == 0x0102);
  return true;
}

bool test_4XNN_skips_when_not_equal() {
  program_counter = 0x0100;
  registers[0x0A] = 0x40;

  opcode_4xnn_handler(0x4A42);

  ASSERT(program_counter == 0x0102);
  return true;
}

bool test_5XY0_skips_when_registers_equal() {
  registers[0x04] = 0x3C;
  registers[0x0B] = 0x3C;
  program_counter = 0x0100;

  opcode_5xy0_handler(0x54B0);

  ASSERT(program_counter == 0x0102);
  return true;
}

bool test_6XNN_loads_immediate() {
  registers[0x04] = 0xFF;

  opcode_6xnn_handler(0x64AC);

  ASSERT(registers[0x04] == 0xAC);
  return true;
}

bool test_7XNN_adds_immediate() {
  registers[0x0A] = 0x34;

  opcode_7xnn_handler(0x7A22);
  ASSERT(registers[0x0A] == 0x56);

  return true;
}

bool test_8XY0_copies_register() {
  registers[0x0B] = 0x23;
  registers[0x04] = 0x1F;

  opcode_8xy0_handler(0x84B0);
  ASSERT(registers[0x04] == registers[0x0B]);
  return true;
}

bool test_8XY1_or_registers() {
  registers[0x0B] = 0x23;
  registers[0x04] = 0x1F;
  uint8_t expected = registers[0x0B] | registers[0x04];

  opcode_8xy1_handler(0x84B1);
  ASSERT(registers[0x04] == expected);
  return true;
}

bool test_8XY2_and_registers() {
  registers[0x0B] = 0x23;
  registers[0x04] = 0x1F;
  uint8_t expected = registers[0x0B] & registers[0x04];

  opcode_8xy2_handler(0x84B1);
  ASSERT(registers[0x04] == expected);
  return true;
}

bool test_8XY3_xor_registers() {
  registers[0x0B] = 0x23;
  registers[0x04] = 0x1F;
  uint8_t expected = registers[0x0B] ^ registers[0x04];

  opcode_8xy3_handler(0x84B1);
  ASSERT(registers[0x04] == expected);
  return true;
}

bool test_8XY4_adds_with_carry() {
  registers[0x05] = 0xFF;
  registers[0x0A] = 0x02;

  opcode_8xy4_handler(0x85A4);
  ASSERT(registers[0x05] == 0x01);
  ASSERT(VF == 1);
  return true;
}

bool test_8XY5_subtracts_sets_borrow() {
  registers[0x05] = 0x10;
  registers[0x0A] = 0x20;

  opcode_8xy5_handler(0x85A5);
  ASSERT(registers[0x05] == 0xF0);
  ASSERT(VF == 0);
  return true;
}

bool test_8XY6_shifts_right() {
  registers[0x0B] = 0x23;
  uint8_t lsb = registers[0x0B] & 0x01;
  uint8_t expected = registers[0x0B] >> 1;

  opcode_8xy6_handler(0x8B46);
  ASSERT(VF == lsb);
  ASSERT(registers[0x0B] == expected);
  return true;
}

bool test_8XY7_subtracts_reverse() {
  registers[0x04] = 0x10;
  registers[0x0B] = 0x30;

  opcode_8xy7_handler(0x84B7);
  ASSERT(registers[0x04] == 0x20);
  ASSERT(VF == 1);

  return true;
}

bool test_8XYE_shifts_left() {
  registers[0x0B] = 0x23;
  uint8_t msb = (registers[0x0B] & 0x80) >> 7;
  uint8_t expected = registers[0x0B] << 1;

  opcode_8xye_handler(0x8B46);
  ASSERT(VF == msb);
  ASSERT(registers[0x0B] == expected);
  return true;
}

bool test_9XY0_skips_when_registers_differ() {
  registers[0x0B] = 0x23;
  registers[0x04] = 0x31;
  program_counter = 0x0100;

  opcode_9xy0_handler(0x84B6);
  ASSERT(program_counter == 0x0102);
  return true;
}

bool test_ANNN_loads_index_register() {
  index_register = 0x0124;

  opcode_annn_handler(0xA6F2);
  ASSERT(index_register == 0x06F2);
  return true;
}

bool test_BNNN_jumps_with_offset() {
  registers[0x00] = 0x24;

  opcode_bnnn_handler(0xB200);
  ASSERT(program_counter == 0x0224);
  return true;
}

bool test_CXNN_random_and_mask() {
  for (int i = 0; i < 40; i++) {
    opcode_cxnn_handler(0xC40F);
    ASSERT(registers[0x04] <= 0x0F);
  }
  return true;
}

bool test_DXYN_draws_sprite() {
  for (int i = 0; i < SW * SH; i++)
    screen[i] = false;

  index_register = 0x0300;
  memory[0x0300] = 0b11110000;
  memory[0x0301] = 0b00001111;

  registers[0x04] = 5;
  registers[0x0B] = 5;

  screen[5 * SW + 6] = true;

  opcode_dxyn_handler(0xD4B2);

  ASSERT(VF == 1);
  ASSERT(screen[5 * SW + 6] == false);

  uint8_t row_pixels = 0;
  for (int col = 0; col < 8; col++) {
    row_pixels |= (screen[5 * SW + 5 + col] << (7 - col));
  }
  ASSERT(row_pixels == 0b10110000);

  return true;
}

bool test_EX9E_skips_on_key_press() {
  keys_pressed[0x0A] = true;
  program_counter = 0x0100;

  opcode_ex9e_handler(0xEA9E);
  ASSERT(program_counter == 0x0102);
  return true;
}

bool test_EXA1_skips_on_key_not_pressed() {
  keys_pressed[0x0B] = false;
  program_counter = 0x0100;

  opcode_exa1_handler(0xEB9E);
  ASSERT(program_counter == 0x0102);
  return true;
}

bool test_FX07_loads_delay_timer() {
  registers[0x08] = 0xF2;
  delay_register = 0x40;

  opcode_fx07_handler(0xF807);
  ASSERT(registers[0x08] == delay_register);
  return true;
}

bool test_FX0A_blocks_until_key() {
  registers[0x0A] = 0xFF;
  program_counter = 0x0108;
  for (int i = 0; i < 16; i++)
    keys_pressed[i] = false;

  opcode_fx0a_handler(0xFA0A);
  ASSERT(program_counter == 0x0106);
  ASSERT(registers[0x0A] != 0x0D);
  opcode_fx0a_handler(0xFA0A);
  ASSERT(program_counter == 0x0104);
  ASSERT(registers[0x0A] != 0x0D);
  opcode_fx0a_handler(0xFA0A);
  ASSERT(program_counter == 0x0102);
  ASSERT(registers[0x0A] != 0x0D);
  opcode_fx0a_handler(0xFA0A);
  ASSERT(program_counter == 0x0100);
  ASSERT(registers[0x0A] != 0x0D);

  keys_pressed[0x0D] = true;

  opcode_fx0a_handler(0xFA0A);
  ASSERT(program_counter == 0x0100);
  ASSERT(registers[0x0A] = 0x0D);
  return true;
}

bool test_FX15_sets_delay_timer() {
  registers[0x08] = 0xF2;
  delay_register = 0x40;

  opcode_fx15_handler(0xF807);
  ASSERT(registers[0x08] == delay_register);
  return true;
}

bool test_FX18_sets_sound_timer() {
  registers[0x08] = 0xF2;
  sound_register = 0x40;

  opcode_fx18_handler(0xF807);
  ASSERT(registers[0x08] == sound_register);
  return true;
}

bool test_FX1E_adds_to_index_register() {
  index_register = 0x0100;
  registers[0x0A] = 0x36;
  uint16_t expected = index_register + registers[0x0A];

  opcode_fx1e_handler(0xFA1E);
  ASSERT(index_register == expected);
  return true;
}

bool test_FX29_points_to_sprite() {
  registers[0x06] = 0x05;

  opcode_fx29_handler(0xF629);
  ASSERT(index_register == registers[0x06] * 0x05);
  return true;
}

bool test_FX33_stores_bcd_representation() {
  index_register = 0x220;
  memory[index_register] = 0;
  memory[index_register + 1] = 0;
  memory[index_register + 2] = 0;

  registers[0x0C] = 125;

  opcode_fx33_handler(0xFC33);
  ASSERT(memory[index_register] == 1);
  ASSERT(memory[index_register + 1] == 2);
  ASSERT(memory[index_register + 2] == 5);
  return true;
}

bool test_FX55_stores_registers() {
  index_register = 0x0300;
  for (int i = 0; i < 15; i++)
    registers[i] = i;
  for (int i = 0; i < 15; i++)
    memory[index_register + i] = 0;

  opcode_fx55_handler(0xFB55);

  bool registers_match_memory = true;
  for (int i = 0; i < 0x0B; i++)
    registers_match_memory &= memory[index_register + i] == registers[i];
  ASSERT(registers_match_memory);
  return true;
}

bool test_FX65_loads_registers() {
  index_register = 0x0100;
  for (int i = 0; i < 15; i++)
    memory[index_register + i] = i;
  for (int i = 0; i < 15; i++)
    registers[i] = 0;

  opcode_fx65_handler(0xFB65);

  bool registers_match_memory = true;
  for (int i = 0; i < 0x0B; i++)
    registers_match_memory &= memory[index_register + i] == registers[i];
  ASSERT(registers_match_memory);
  return true;
}

int test_opcodes() {
  printf("Starting tests for opcodes\n\n");

  RUN_TEST(opcode,test_00E0_clears_full_screen);
  RUN_TEST(opcode,test_00EE_restores_address);
  RUN_TEST(opcode,test_1NNN_moves_pc_to_inmediate);
  RUN_TEST(opcode,test_2NNN_moves_pc_and_stores_inmediate);
  RUN_TEST(opcode,test_3XNN_skips_when_equal);
  RUN_TEST(opcode,test_4XNN_skips_when_not_equal);
  RUN_TEST(opcode,test_5XY0_skips_when_registers_equal);
  RUN_TEST(opcode,test_6XNN_loads_immediate);
  RUN_TEST(opcode,test_7XNN_adds_immediate);
  RUN_TEST(opcode,test_8XY0_copies_register);
  RUN_TEST(opcode,test_8XY1_or_registers);
  RUN_TEST(opcode,test_8XY2_and_registers);
  RUN_TEST(opcode,test_8XY3_xor_registers);
  RUN_TEST(opcode,test_8XY4_adds_with_carry);
  RUN_TEST(opcode,test_8XY5_subtracts_sets_borrow);
  RUN_TEST(opcode,test_8XY6_shifts_right);
  RUN_TEST(opcode,test_8XY7_subtracts_reverse);
  RUN_TEST(opcode,test_8XYE_shifts_left);
  RUN_TEST(opcode,test_9XY0_skips_when_registers_differ);
  RUN_TEST(opcode,test_ANNN_loads_index_register);
  RUN_TEST(opcode,test_BNNN_jumps_with_offset);
  RUN_TEST(opcode,test_CXNN_random_and_mask);
  RUN_TEST(opcode,test_DXYN_draws_sprite);
  RUN_TEST(opcode,test_EX9E_skips_on_key_press);
  RUN_TEST(opcode,test_EXA1_skips_on_key_not_pressed);
  RUN_TEST(opcode,test_FX07_loads_delay_timer);
  RUN_TEST(opcode,test_FX0A_blocks_until_key);
  RUN_TEST(opcode,test_FX15_sets_delay_timer);
  RUN_TEST(opcode,test_FX18_sets_sound_timer);
  RUN_TEST(opcode,test_FX1E_adds_to_index_register);
  RUN_TEST(opcode,test_FX29_points_to_sprite);
  RUN_TEST(opcode,test_FX33_stores_bcd_representation);
  RUN_TEST(opcode,test_FX55_stores_registers);
  RUN_TEST(opcode,test_FX65_loads_registers);

  PRINT_TEST_SUMMARY(opcode);
}
