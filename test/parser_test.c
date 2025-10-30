#include "parser.h"
#include "test_utils.h"
#include <stdatomic.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <sys/types.h>

int parser_successful_tests = 0;
int parser_total_tests = 0;

bool test_parser_00E0() {
    ASSERT(get_opcode_identifier(0x00E0) == OPCODE_00E0);
    return true;
}

bool test_parser_00EE() {
    ASSERT(get_opcode_identifier(0x00EE) == OPCODE_00EE);
    return true;
}

bool test_parser_1NNN() {
    ASSERT(get_opcode_identifier(0x1234) == OPCODE_1NNN);
    ASSERT(get_opcode_identifier(0x1FFF) == OPCODE_1NNN);
    return true;
}

bool test_parser_8XY4() {
    ASSERT(get_opcode_identifier(0x8124) == OPCODE_8XY4);
    return true;
}

bool test_parser_8XYE() {
    ASSERT(get_opcode_identifier(0x8ABC) == -1);
    ASSERT(get_opcode_identifier(0x8ABF) == -1);
    ASSERT(get_opcode_identifier(0x8ABE) == OPCODE_8XYE);
    return true;
}

bool test_parser_FX55() {
    ASSERT(get_opcode_identifier(0xF055) == OPCODE_FX55);
    ASSERT(get_opcode_identifier(0xFA55) == OPCODE_FX55);
    ASSERT(get_opcode_identifier(0xFF55) == OPCODE_FX55);
    return true;
}

bool test_parser_invalid_5XYN() {
    ASSERT(get_opcode_identifier(0x5120) == OPCODE_5XY0);
    ASSERT(get_opcode_identifier(0x5121) == -1);
    ASSERT(get_opcode_identifier(0x5122) == -1);
    return true;
}

bool test_parser_invalid_9XYN() {
    ASSERT(get_opcode_identifier(0x9120) == OPCODE_9XY0);
    ASSERT(get_opcode_identifier(0x9121) == -1);
    return true;
}



void test_parser() {
    printf("Inisio del tes del sarper\n");

    RUN_TEST(test_parser_00E0);
    RUN_TEST(test_parser_00EE);
    RUN_TEST(test_parser_1NNN);
    RUN_TEST(test_parser_8XY4);
    RUN_TEST(test_parser_8XYE);
    RUN_TEST(test_parser_FX55);
    RUN_TEST(test_parser_invalid_5XYN);
    RUN_TEST(test_parser_invalid_9XYN);

    printf("\nSuccessful tests: %d/%d\n", parser_successful_tests,
           parser_total_tests);

    if (parser_successful_tests == parser_total_tests) {
        printf("All tests passed!\n");
    } else {
        printf("SOME TESTS FAILED!\n");
    }
}
