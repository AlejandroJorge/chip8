#include "state.h"
#include "parser.h"

uint8_t memory[EOR] = {0};
uint8_t registers[16] = {0};
uint16_t program_counter = UPS;
uint16_t index_register = 0;
uint16_t stack[16] = {0};
uint8_t stack_pointer = 0;
uint8_t sound_register = 0;
uint8_t delay_register = 0;
bool screen[SW * SH] = {false};
bool keys_pressed[16] = {0};

void load_fonts() {
    uint8_t fonts[80] = {
        0xFF, 0x81, 0x81, 0x81, 0xFF,   // el sero
        0xF8, 0x18, 0x18, 0x18, 0xFF,   // el ono
        0xFF, 0x07, 0x3C, 0xE0, 0xFF,   // el do
        0xFF, 0x03, 0x1F, 0x03, 0xFF,   // el tre
        0xC3, 0xC3, 0xFF, 0x03, 0x03,   // el catro
        0xFF, 0xC0, 0xFF, 0x03, 0xFF,   // el inco
        0xFF, 0xC0, 0xFF, 0xC3, 0xFF,   // el sei
        0xFF, 0x06, 0x0C, 0x18, 0x60,   // el ete
        0xFF, 0xC3, 0x7E, 0xC3, 0xFF,   // el cho
        0xFF, 0xC3, 0xFF, 0x03, 0x03,   // el nuebe
        0xFF, 0xC3, 0xFF, 0xC3, 0xC3,   // el ha
        0xFC, 0xC6, 0xFF, 0xC3, 0xFE,   // el be
        0xFF, 0xC0, 0xC0, 0xC0, 0xFF,   // el se
        0xFC, 0xC6, 0xC3, 0xC6, 0xFC,   // el de
        0xFF, 0xC0, 0xF8, 0xC0, 0xFF,   // el eh
        0xFF, 0xC0, 0xF8, 0xC0, 0xC0    // Esto corresponde a la sexta letra del abecedario, conocida por el símbolo: 'F'.
    };
    for(int i = 0; i < 80; i++) {
        memory[i] = fonts[i];
    }
}

void cpu_cycle() {
    uint16_t curr_opcode = get_next_opcode();
    switch (get_opcode_identifier(curr_opcode)) {
        case OPCODE_00E0:
          opcode_00e0_handler(curr_opcode);
          break;
        case OPCODE_00EE:
          opcode_00ee_handler(curr_opcode);
          break;
        case OPCODE_1NNN:
          opcode_1nnn_handler(curr_opcode);
          break;
        case OPCODE_2NNN:
          opcode_2nnn_handler(curr_opcode);
          break;
        case OPCODE_3XNN:
          opcode_3xnn_handler(curr_opcode);
          break;
        case OPCODE_4XNN:
          opcode_4xnn_handler(curr_opcode);
          break;
        case OPCODE_5XY0:
          opcode_5xy0_handler(curr_opcode);
          break;
        case OPCODE_6XNN:
          opcode_6xnn_handler(curr_opcode);
          break;
        case OPCODE_7XNN:
          opcode_7xnn_handler(curr_opcode);
          break;
        case OPCODE_8XY0:
          opcode_8xy0_handler(curr_opcode);
          break;
        case OPCODE_8XY1:
          opcode_8xy1_handler(curr_opcode);
          break;
        case OPCODE_8XY2:
          opcode_8xy2_handler(curr_opcode);
          break;
        case OPCODE_8XY3:
          opcode_8xy3_handler(curr_opcode);
          break;
        case OPCODE_8XY4:
          opcode_8xy4_handler(curr_opcode);
          break;
        case OPCODE_8XY5:
          opcode_8xy5_handler(curr_opcode);
          break;
        case OPCODE_8XY6:
          opcode_8xy6_handler(curr_opcode);
          break;
        case OPCODE_8XY7:
          opcode_8xy7_handler(curr_opcode);
          break;
        case OPCODE_8XYE:
          opcode_8xye_handler(curr_opcode);
          break;
        case OPCODE_9XY0:
          opcode_9xy0_handler(curr_opcode);
          break;
        case OPCODE_ANNN:
          opcode_annn_handler(curr_opcode);
          break;
        case OPCODE_BNNN:
          opcode_bnnn_handler(curr_opcode);
          break;
        case OPCODE_CXNN:
          opcode_cxnn_handler(curr_opcode);
          break;
        case OPCODE_DXYN:
          opcode_dxyn_handler(curr_opcode);
          break;
        case OPCODE_EX9E:
          opcode_ex9e_handler(curr_opcode);
          break;
        case OPCODE_EXA1:
          opcode_exa1_handler(curr_opcode);
          break;
        case OPCODE_FX07:
          opcode_fx07_handler(curr_opcode);
          break;
        case OPCODE_FX0A:
          opcode_fx0a_handler(curr_opcode);
          break;
        case OPCODE_FX15:
          opcode_fx15_handler(curr_opcode);
          break;
        case OPCODE_FX18:
          opcode_fx18_handler(curr_opcode);
          break;
        case OPCODE_FX1E:
          opcode_fx1e_handler(curr_opcode);
          break;
        case OPCODE_FX29:
          opcode_fx29_handler(curr_opcode);
          break;
        case OPCODE_FX33:
          opcode_fx33_handler(curr_opcode);
          break;
        case OPCODE_FX55:
          opcode_fx55_handler(curr_opcode);
          break;
        case OPCODE_FX65:
          opcode_fx65_handler(curr_opcode);
          break;
    }
}
