#include <math.h>
#include <raylib.h>

#include "screen.h"
#include "state.h"

#define SCALE 20
#define FPS 60

#define MAX_SAMPLES_PER_UPDATE 4096
#define MAX_SAMPLES 512
#define SAMPLE_RATE 44100
#define SAMPLE_SIZE 16

#define CPU_HZ 600
#define CPU_TIME_STEP (1.0f / CPU_HZ)

float audioFrequency = 440.f;
float frequency = 440.f;
float sineIdx = 0.0f;

void AudioInputCallback(void *buffer, unsigned int frames) {

  audioFrequency = frequency + (audioFrequency - frequency) * 0.95f;

  float incr = audioFrequency / 44100.0f;
  short *d = (short *)buffer;

  for (unsigned int i = 0; i < frames; i++) {
    d[i] = (short)(32000.0f * sinf(2 * PI * sineIdx));
    sineIdx += incr;
    if (sineIdx > 1.0f)
      sineIdx -= 1.0f;
  }
}

void init_screen() {
  const int screen_width = SW * SCALE;
  const int screen_height = SH * SCALE;

  InitWindow(screen_width, screen_height, "Chimpocho");

  SetTargetFPS(FPS);

  InitAudioDevice();

  SetAudioStreamBufferSizeDefault(MAX_SAMPLES_PER_UPDATE);

  AudioStream stream = LoadAudioStream(SAMPLE_RATE, SAMPLE_SIZE, 1);

  SetAudioStreamCallback(stream, AudioInputCallback);

  Rectangle screen_pixels[SW * SH];
  for (int i = 0; i < SW; i++) {
    for (int j = 0; j < SH; j++) {
      Rectangle r;
      r.x = i * SCALE;
      r.y = j * SCALE;
      r.height = SCALE;
      r.width = SCALE;
      screen_pixels[i + j * SW] = r;
    }
  }

  float cum_time = 0.0f;
  while (!WindowShouldClose()) {
    cum_time += GetFrameTime();

    if (IsKeyDown(KEY_ONE))
      keys_pressed[0] = true;
    if (IsKeyDown(KEY_TWO))
      keys_pressed[1] = true;
    if (IsKeyDown(KEY_THREE))
      keys_pressed[2] = true;
    if (IsKeyDown(KEY_FOUR))
      keys_pressed[3] = true;
    if (IsKeyDown(KEY_Q))
      keys_pressed[4] = true;
    if (IsKeyDown(KEY_W))
      keys_pressed[5] = true;
    if (IsKeyDown(KEY_E))
      keys_pressed[6] = true;
    if (IsKeyDown(KEY_R))
      keys_pressed[7] = true;
    if (IsKeyDown(KEY_A))
      keys_pressed[8] = true;
    if (IsKeyDown(KEY_S))
      keys_pressed[9] = true;
    if (IsKeyDown(KEY_D))
      keys_pressed[10] = true;
    if (IsKeyDown(KEY_F))
      keys_pressed[11] = true;
    if (IsKeyDown(KEY_Z))
      keys_pressed[12] = true;
    if (IsKeyDown(KEY_X))
      keys_pressed[13] = true;
    if (IsKeyDown(KEY_C))
      keys_pressed[14] = true;
    if (IsKeyDown(KEY_V))
      keys_pressed[15] = true;

    if (IsKeyUp(KEY_ONE))
      keys_pressed[0] = false;
    if (IsKeyUp(KEY_TWO))
      keys_pressed[1] = false;
    if (IsKeyUp(KEY_THREE))
      keys_pressed[2] = false;
    if (IsKeyUp(KEY_FOUR))
      keys_pressed[3] = false;
    if (IsKeyUp(KEY_Q))
      keys_pressed[4] = false;
    if (IsKeyUp(KEY_W))
      keys_pressed[5] = false;
    if (IsKeyUp(KEY_E))
      keys_pressed[6] = false;
    if (IsKeyUp(KEY_R))
      keys_pressed[7] = false;
    if (IsKeyUp(KEY_A))
      keys_pressed[8] = false;
    if (IsKeyUp(KEY_S))
      keys_pressed[9] = false;
    if (IsKeyUp(KEY_D))
      keys_pressed[10] = false;
    if (IsKeyUp(KEY_F))
      keys_pressed[11] = false;
    if (IsKeyUp(KEY_Z))
      keys_pressed[12] = false;
    if (IsKeyUp(KEY_X))
      keys_pressed[13] = false;
    if (IsKeyUp(KEY_C))
      keys_pressed[14] = false;
    if (IsKeyUp(KEY_V))
      keys_pressed[15] = false;

    while (cum_time >= CPU_TIME_STEP) {
      cpu_cycle();
      cum_time -= CPU_TIME_STEP;
    }

    BeginDrawing();
    ClearBackground(BLACK);

    for (int i = 0; i < SW * SH; i++) {
      if (screen[i])
        DrawRectangleRec(screen_pixels[i], WHITE);
    }

    EndDrawing();

    if (delay_register > 0)
      delay_register--;
    if (sound_register > 0) {
      PlayAudioStream(stream);
      sound_register--;
      if (sound_register == 0) {
        StopAudioStream(stream);
      }
    }
  }

  UnloadAudioStream(stream);

  CloseAudioDevice();

  CloseWindow();
}
