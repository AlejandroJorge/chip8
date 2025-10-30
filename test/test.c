#include "opcodes_test.h"
#include "parser_test.h"
#include <stdio.h>

int main(void) {
  int opcodes_failed_tests = test_opcodes();
  int parser_failed_tests = test_parser();

  printf("\n-------------------------------\n");
  if (opcodes_failed_tests > 0)
    printf("Opcodes failed tests: %d\n", opcodes_failed_tests);
  else
    printf("All Opcodes tests passed\n");
  if (parser_failed_tests > 0)
    printf("Parser failed tests: %d\n", parser_failed_tests);
  else
    printf("All parser tests passed\n");
}
