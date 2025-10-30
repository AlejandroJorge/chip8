#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>

#include "parser.h"
#include "screen.h"
#include "state.h"

int main(int argc, char *argv[]) {

    if(argc != 2) {
        perror("Eres mierda\n");
        exit(1);
    }

    srand(0);
    load_rom(argv[1]);
    load_fonts();
    init_screen();

    return 0;
}
