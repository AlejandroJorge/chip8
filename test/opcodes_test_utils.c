#include "state.h"

void set_screen(bool value) {
  for (int i = 0; i < SW * SH; i++) {
    screen[i] = value;
  }
}

bool is_screen_clear() {
  for (int i = 0; i < SW * SH; i++) {
    if (screen[i]) return false;
  }
  return true;
}

