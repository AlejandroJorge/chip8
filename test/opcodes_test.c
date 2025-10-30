#include "opcodes_test.h"
#include "opcodes.h"
#include "state.h"
#include "test_utils.h"
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <sys/types.h>

// Important for macro definition
int opcode_successful_tests = 0;
int opcode_total_tests = 0;

bool test_00E0_clears_full_screen() {
  // Fill screen with 1's
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
  stack[stack_pointer-2] = 0x0120;

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

  opcode_2nnn_handler(0x2124);
  ASSERT(stack_pointer == 4);
  ASSERT(stack[stack_pointer - 2] == 0x124);
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
  registers[0x0A] = 0x0D;
  
  opcode_7xnn_handler(0x7A52);
  ASSERT(registers[0x0A] == 0x60);

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

// TODO: Implement
bool test_8XY2_and_registers() { return false; }

// TODO: Implement
bool test_8XY3_xor_registers() { return false; }

// TODO: Implement
bool test_8XY4_adds_with_carry() { return false; }

// TODO: Implement
bool test_8XY5_subtracts_sets_borrow() { return false; }

// TODO: Implement
bool test_8XY6_shifts_right() { return false; }

// TODO: Implement
bool test_8XY7_subtracts_reverse() { return false; }

// TODO: Implement
bool test_8XYE_shifts_left() { return false; }

// TODO: Implement
bool test_9XY0_skips_when_registers_differ() { return false; }

// TODO: Implement
bool test_ANNN_loads_index_register() { return false; }

// TODO: Implement
bool test_BNNN_jumps_with_offset() { return false; }

// TODO: Implement
bool test_CXNN_random_and_mask() { return false; }

// TODO: Implement
bool test_DXYN_draws_sprite() { return false; }

// TODO: Implement
bool test_EX9E_skips_on_key_press() { return false; }

// TODO: Implement
bool test_EXA1_skips_on_key_not_pressed() { return false; }

// TODO: Implement
bool test_FX07_loads_delay_timer() { return false; }

// TODO: Implement
bool test_FX0A_blocks_until_key() { return false; }

// TODO: Implement
bool test_FX15_sets_delay_timer() { return false; }

// TODO: Implement
bool test_FX18_sets_sound_timer() { return false; }

// TODO: Implement
bool test_FX1E_adds_to_index_register() { return false; }

// TODO: Implement
bool test_FX29_points_to_sprite() { return false; }

// TODO: Implement
bool test_FX33_stores_bcd_representation() { return false; }

// TODO: Implement
bool test_FX55_stores_registers() { return false; }

// TODO: Implement
bool test_FX65_loads_registers() { return false; }

void test_opcodes() {
  printf("Starting tests for opcodes\n\n");

  RUN_TEST(test_00E0_clears_full_screen);
  RUN_TEST(test_00EE_restores_address);
  RUN_TEST(test_1NNN_moves_pc_to_inmediate);
  RUN_TEST(test_2NNN_moves_pc_and_stores_inmediate);
  RUN_TEST(test_3XNN_skips_when_equal);
  RUN_TEST(test_4XNN_skips_when_not_equal);
  RUN_TEST(test_5XY0_skips_when_registers_equal);
  RUN_TEST(test_6XNN_loads_immediate);
  RUN_TEST(test_7XNN_adds_immediate);
  RUN_TEST(test_8XY0_copies_register);
  RUN_TEST(test_8XY1_or_registers);
  RUN_TEST(test_8XY2_and_registers);
  RUN_TEST(test_8XY3_xor_registers);
  RUN_TEST(test_8XY4_adds_with_carry);
  RUN_TEST(test_8XY5_subtracts_sets_borrow);
  RUN_TEST(test_8XY6_shifts_right);
  RUN_TEST(test_8XY7_subtracts_reverse);
  RUN_TEST(test_8XYE_shifts_left);
  RUN_TEST(test_9XY0_skips_when_registers_differ);
  RUN_TEST(test_ANNN_loads_index_register);
  RUN_TEST(test_BNNN_jumps_with_offset);
  RUN_TEST(test_CXNN_random_and_mask);
  RUN_TEST(test_DXYN_draws_sprite);
  RUN_TEST(test_EX9E_skips_on_key_press);
  RUN_TEST(test_EXA1_skips_on_key_not_pressed);
  RUN_TEST(test_FX07_loads_delay_timer);
  RUN_TEST(test_FX0A_blocks_until_key);
  RUN_TEST(test_FX15_sets_delay_timer);
  RUN_TEST(test_FX18_sets_sound_timer);
  RUN_TEST(test_FX1E_adds_to_index_register);
  RUN_TEST(test_FX29_points_to_sprite);
  RUN_TEST(test_FX33_stores_bcd_representation);
  RUN_TEST(test_FX55_stores_registers);
  RUN_TEST(test_FX65_loads_registers);

  printf("\nSuccessful tests: %d/%d\n", opcode_successful_tests,
         opcode_total_tests);

  if (opcode_successful_tests == opcode_total_tests) {
    printf("All tests passed!\n");
  } else {
    printf("SOME TESTS FAILED!\n");
  }
}
