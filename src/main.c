#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

#include "parser.h"
#include "screen.h"
#include "state.h"

int main() {

  srand(0);
  load_rom("example.c8");
  load_fonts();
  init_screen();

  return 0;
}
