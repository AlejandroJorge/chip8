#pragma once

#define ASSERT(condition)                                                      \
  do {                                                                         \
    if (!(condition)) {                                                        \
      printf("  [ASSERT FAILED] %s at %s:%d\n", #condition, __FILE__,          \
             __LINE__);                                                        \
      return false;                                                            \
    }                                                                          \
  } while (0)

#define RUN_TEST(test_func)                                                    \
  do {                                                                         \
    opcode_total_tests++;                                                      \
    printf("[%s] Starting test...\n", #test_func);                             \
    if (test_func()) {                                                         \
      opcode_successful_tests++;                                               \
      printf("[%s] ✓ PASSED\n\n", #test_func);                                 \
    } else {                                                                   \
      printf("[%s] ✗ FAILED\n\n", #test_func);                                 \
    }                                                                          \
  } while (0)
