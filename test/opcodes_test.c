#include "opcodes.h"
#include "test_utils.h"
#include "opcodes_test_utils.h"
#include "opcodes_test.h"
#include <stdint.h>
#include <stdio.h>
#include <stdbool.h>

// Important for macro definition
int opcode_successful_tests = 0;
int opcode_total_tests = 0;

bool test_00E0_clears_full_screen() {
  set_screen(true);
  opcode_00e0_handler(0x0000);
  ASSERT(is_screen_clear());
  return true;
}

bool test_00E0_clears_empty_screen() {
  set_screen(false);
  opcode_00e0_handler(0x0000);
  ASSERT(is_screen_clear());
  return true;
}

void test_opcodes() {
  printf("[OPCODE] Starting tests for opcodes\n");
  printf("-----------------------------------\n");

  RUN_TEST(test_00E0_clears_full_screen, "00E0: Should clear a full screen");
  RUN_TEST(test_00E0_clears_empty_screen, "00E0: Should work on an empty screen");

  printf("-----------------------------------\n");
  printf("[OPCODE] Successful tests: %d/%d\n",
         opcode_successful_tests, opcode_total_tests);
  
  if (opcode_successful_tests == opcode_total_tests) {
    printf("[OPCODE] All tests passed!\n");
  } else {
    printf("[OPCODE] SOME TESTS FAILED!\n");
  }
}
