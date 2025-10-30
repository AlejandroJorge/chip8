#include "opcodes.h"
#include "state.h"
#include <stdint.h>
#include <stdio.h>

int opcode_successful_tests = 0;
int opcode_total_tests = 0;

// TODO: Codex, implement at least one test for each opcode

// Testing Opcode 00e0: Test 01
void test_opcode_00e0_T01() {
  opcode_total_tests++;
  printf("[TEST] [00E0] [T01] Starting test...\n");

  // Precondition: screen is full
  for (int i = 0; i < SW; i++)
    for (int j = 0; j < SH; j++)
      screen[j * SW + i] = true;

  uint16_t opcode = 0x0000;
  opcode_00e0_handler(opcode);

  bool is_screen_empty = true;
  for (int i = 0; i < SW; i++)
    for (int j = 0; j < SH; j++)
      is_screen_empty &= !screen[j * SW + i];

  // Postcondition: screen is empty
  if (!is_screen_empty) {
    printf("[TEST] [00E0] [T01] Test failed\n");
    return;
  }

  opcode_successful_tests++;
  printf("[TEST] [00E0] [T01] Successful test\n");
}

// Testing Opcode 00e0: Test 02
void test_opcode_00e0_T02() {
  opcode_total_tests++;
  printf("[TEST] [00E0] [T01] Starting test...\n");

  // Precondition: screen is empty
  for (int i = 0; i < SW; i++)
    for (int j = 0; j < SH; j++)
      screen[j * SW + i] = false;

  uint16_t opcode = 0x0000;
  opcode_00e0_handler(opcode);

  bool is_screen_empty = true;
  for (int i = 0; i < SW; i++)
    for (int j = 0; j < SH; j++)
      is_screen_empty &= !screen[j * SW + i];

  // Postcondition: screen is empty
  if (!is_screen_empty) {
    printf("[TEST] [00E0] [T02] Test failed\n");
    return;
  }

  opcode_successful_tests++;
  printf("[TEST] [00E0] [T02] Successful test\n");
}

// Entry point of testing all opcodes
void test_opcodes() {
  printf("[TEST] [OPCODE] Starting tests for opcodes\n");
  printf("------------------------------------------\n");

  // Opcode 00e0
  test_opcode_00e0_T01();
  test_opcode_00e0_T02();

  printf("------------------------------------------\n");
  printf("[TEST] [OPCODE] Successful tests: %d/%d\n",
         opcode_successful_tests, opcode_total_tests);
  if (opcode_successful_tests == opcode_total_tests) 
    printf("[TEST] [OPCODE] All test passed!\n");
}
