#include <raylib.h>

#include "screen.h"
#include "state.h"

#define SCALE 20

void init_screen() {
    const int screen_width = SW * SCALE;
    const int screen_height = SH * SCALE;

    InitWindow(screen_width, screen_height, "Chimpocho");

    Rectangle screen_pixels[SW*SH];
    for(int i = 0; i < SW; i++) {
        for(int j = 0; j < SH; j++) {
            Rectangle r; r.x = i*SCALE; r.y = j*SCALE; r.height = SCALE; r.width = SCALE;
            screen_pixels[i+j*SW] = r;
        }
    }

    while(!WindowShouldClose()) {
        BeginDrawing();
        for(int i = 0; i < SW*SH; i++) {
            DrawRectangleRec(screen_pixels[i], screen[i] ? GREEN : WHITE);
        }

        // TODO: Implement ts in another thread ahahahah
        if(IsKeyDown(KEY_ONE)) keys_pressed[0] = true;
        if(IsKeyDown(KEY_TWO)) keys_pressed[1] = true;
        if(IsKeyDown(KEY_THREE)) keys_pressed[2] = true;
        if(IsKeyDown(KEY_FOUR)) keys_pressed[3] = true;
        if(IsKeyDown(KEY_Q)) keys_pressed[4] = true;
        if(IsKeyDown(KEY_W)) keys_pressed[5] = true;
        if(IsKeyDown(KEY_E)) keys_pressed[6] = true;
        if(IsKeyDown(KEY_R)) keys_pressed[7] = true;
        if(IsKeyDown(KEY_A)) keys_pressed[8] = true;
        if(IsKeyDown(KEY_S)) keys_pressed[9] = true;
        if(IsKeyDown(KEY_D)) keys_pressed[10] = true;
        if(IsKeyDown(KEY_F)) keys_pressed[11] = true;
        if(IsKeyDown(KEY_Z)) keys_pressed[12] = true;
        if(IsKeyDown(KEY_X)) keys_pressed[13] = true;
        if(IsKeyDown(KEY_C)) keys_pressed[14] = true;
        if(IsKeyDown(KEY_V)) keys_pressed[15] = true;

        if(IsKeyUp(KEY_ONE)) keys_pressed[0] = false;
        if(IsKeyUp(KEY_TWO)) keys_pressed[1] = false;
        if(IsKeyUp(KEY_THREE)) keys_pressed[2] = false;
        if(IsKeyUp(KEY_FOUR)) keys_pressed[3] = false;
        if(IsKeyUp(KEY_Q)) keys_pressed[4] = false;
        if(IsKeyUp(KEY_W)) keys_pressed[5] = false;
        if(IsKeyUp(KEY_E)) keys_pressed[6] = false;
        if(IsKeyUp(KEY_R)) keys_pressed[7] = false;
        if(IsKeyUp(KEY_A)) keys_pressed[8] = false;
        if(IsKeyUp(KEY_S)) keys_pressed[9] = false;
        if(IsKeyUp(KEY_D)) keys_pressed[10] = false;
        if(IsKeyUp(KEY_F)) keys_pressed[11] = false;
        if(IsKeyUp(KEY_Z)) keys_pressed[12] = false;
        if(IsKeyUp(KEY_X)) keys_pressed[13] = false;
        if(IsKeyUp(KEY_C)) keys_pressed[14] = false;
        if(IsKeyUp(KEY_V)) keys_pressed[15] = false;

        EndDrawing();
    }

    CloseWindow();
}
