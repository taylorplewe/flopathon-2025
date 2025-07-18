// 2025 Taylor Plewe
//
// build with:
//   emcc draw.c -o a.out.js \
//   -O3 \
//   -s USE_SDL=2 \
//   -s MODULARIZE \
//   -s EXPORT_ES6 \
//   -s EXPORT_NAME=draw \
//   -s EXPORTED_FUNCTIONS='_get_match_percentage,_main' \
//   -s EXPORTED_RUNTIME_METHODS='cwrap,ccall'"

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <emscripten.h>
#include <emscripten/html5.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define WIDTH 128
#define HEIGHT 128
#define BPP 32
#define COL_BG 0xffffff
#define COL_FG 0x000000
#define PENCIL_MAX_RADIUS 28
#define PENCIL_MIN_RADIUS 0

Uint8 draw_pixels[WIDTH * HEIGHT * (BPP/8)];

SDL_Window* window;
SDL_Renderer* renderer;
SDL_Surface* surface;
SDL_Surface* target_surface;

int pencil_radius = (PENCIL_MAX_RADIUS - PENCIL_MIN_RADIUS) / 2;
int sdl_x, sdl_y;

enum MouseButton {
  MouseButtonNone,
  MouseButtonLeft,
  MouseButtonRight
};
enum MouseButton mouse_button_down = MouseButtonNone;

void fill_circle(Uint8* mem, int x, int y, int r, Uint32 color) {
  for (int w = 0; w <= r*2; w++) {
    for (int h = 0; h <= r*2; h++) {
      int dx = r - w;
      int dy = r - h;
      int offset_x = (dx + x) % WIDTH;
      if (
        ((dx*dx) + (dy*dy) > r*r)
        || (offset_x < 0)
        || ((dx + x) >= WIDTH)
      )
        continue;
      
      // get corresponding index into memory
      int index = (
        ((dy + y) * WIDTH) + offset_x
      ) * (BPP/8);
      if (index < 0) continue;
      if (index > WIDTH * HEIGHT * (BPP/8)) continue;

      // draw provided color at point
      mem[index] = color & 0xff;
      mem[index+1] = (color >> 8) & 0xff;
      mem[index+2] = (color >> 16) & 0xff;
    }
  }
}

void update() {
  // SDL_GetGlobalMouseState(&sdl_x, &sdl_y);
  SDL_RenderClear(renderer);

  // make modifications to surface
  if (SDL_MUSTLOCK(surface)) SDL_LockSurface(surface);

  // first, draw everything the user has drawn thus far
  memcpy(surface->pixels, draw_pixels, WIDTH * HEIGHT * (BPP/8));

  // second, draw the user's cursor on top of that
  fill_circle((Uint8*)surface->pixels, sdl_x, sdl_y, pencil_radius, 0x88888888);

  // draw eisntein
  SDL_Rect rect = { 0, 0, target_surface->w, target_surface->h };
  SDL_BlitSurface(target_surface, NULL, surface, &rect);

  if (SDL_MUSTLOCK(surface)) SDL_UnlockSurface(surface);

  // pencil update logic
  if (mouse_button_down != MouseButtonNone) {
    Uint32 col = mouse_button_down == MouseButtonLeft ? COL_FG : COL_BG;
    fill_circle(draw_pixels, sdl_x, sdl_y, pencil_radius, col);
  }

  // draw surface to screen using renderer
  SDL_Texture* screenTexture = SDL_CreateTextureFromSurface(renderer, surface);

  SDL_RenderCopy(renderer, screenTexture, NULL, NULL);
  SDL_RenderPresent(renderer);

  SDL_DestroyTexture(screenTexture);
}

bool mouse_callback(int eventType, const EmscriptenMouseEvent* e, void* userData) {
  if (eventType == EMSCRIPTEN_EVENT_MOUSEDOWN)
    mouse_button_down = e->button == 0
      ? MouseButtonLeft
      : MouseButtonRight;
  else if (eventType == EMSCRIPTEN_EVENT_MOUSEUP) mouse_button_down = MouseButtonNone;
  else if (eventType == EMSCRIPTEN_EVENT_MOUSEMOVE) {
    sdl_x = e->targetX / 3;
    sdl_y = e->targetY / 3;
  }
  return 0;
}

bool wheel_callback(int eventType, const EmscriptenWheelEvent* e, void* userData) {
  pencil_radius += (int)(e->deltaY/2);
  if (pencil_radius > PENCIL_MAX_RADIUS) pencil_radius = PENCIL_MAX_RADIUS;
  if (pencil_radius < PENCIL_MIN_RADIUS) pencil_radius = PENCIL_MIN_RADIUS;
  return 0;
}

float match_percentage = 0.0f;
EMSCRIPTEN_KEEPALIVE
float get_match_percentage() {
  int num_matched = 0;
  Uint8* target_pixels = (Uint8*)target_surface->pixels;
  for (int i = 0; i < WIDTH * HEIGHT * (BPP/8); i++) {
    if (draw_pixels[i] == target_pixels[i]) num_matched++;
  }
  return (float)num_matched / (WIDTH * HEIGHT * (BPP/8));
}

int main(int argc, char** argv) {
  memset(draw_pixels, COL_BG & 0xff, WIDTH * HEIGHT * (BPP/8));
  
  SDL_Init(SDL_INIT_VIDEO);
  SDL_CreateWindowAndRenderer(WIDTH, HEIGHT, 0, &window, &renderer);
  surface = SDL_CreateRGBSurface(0, WIDTH, HEIGHT, BPP, 0, 0, 0, 0);

  printf("ehre we go\n");
  target_surface = IMG_Load("einstein.bmp");
  if (!target_surface) {
    printf("uh oh\n");
    printf("%s\n", IMG_GetError());
  }

  emscripten_set_mousedown_callback(EMSCRIPTEN_EVENT_TARGET_WINDOW, 0, 1, mouse_callback);
  emscripten_set_mouseup_callback(EMSCRIPTEN_EVENT_TARGET_WINDOW, 0, 1, mouse_callback);
  emscripten_set_mousemove_callback("canvas#drawable", 0, 1, mouse_callback);
  emscripten_set_wheel_callback(EMSCRIPTEN_EVENT_TARGET_WINDOW, 0, 1, wheel_callback);
  emscripten_set_main_loop(update, 0, 1);
}
