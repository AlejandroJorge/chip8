#pragma once

#define ASSERT(condition)                                                      \
  do {                                                                         \
    if (!(condition)) {                                                        \
      printf("  [ASSERT FAILED] %s at %s:%d\n", #condition, __FILE__,          \
             __LINE__);                                                        \
      return false;                                                            \
    }                                                                          \
  } while (0)

#define RUN_TEST(test_func, test_name)                                         \
  do {                                                                         \
    opcode_total_tests++;                                                      \
    printf("[%s] Starting...\n", test_name);                            \
    if (test_func()) {                                                         \
      opcode_successful_tests++;                                               \
      printf("[%s] ✓ PASSED\n", test_name);                             \
    } else {                                                                   \
      printf("[%s] ✗ FAILED\n", test_name);                             \
    }                                                                          \
  } while (0)
