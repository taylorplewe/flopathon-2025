#include <SDL.h>
#include <emscripten.h>
#include <emscripten/html5.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define WIDTH 300
#define HEIGHT 400
#define BPP 24

Uint8 draw_pixels[WIDTH * HEIGHT * (BPP/8)];

SDL_Window* window;
SDL_Renderer* renderer;
SDL_Surface* surface;
SDL_Surface* einstein;

int sdl_x, sdl_y;
enum MouseButton {
  MouseButtonNone,
  MouseButtonLeft,
  MouseButtonRight
};
enum MouseButton mouse_button_down = MouseButtonNone;

void fill_circle2(Uint8* mem, int x, int y, int r, Uint32 color) {
  for (int w = 0; w < r*2; w++) {
    for (int h = 0; h < r*2; h++) {
      int dx = r - w;
      int dy = r - h;
      int offset_x = (dx + x) % WIDTH;
      if (offset_x < 0 || offset_x >= WIDTH) continue;
      if (
        ((dx*dx) + (dy*dy) >= r*r)
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

      // draw in provided color at point
      mem[index] = color & 0xff;
      mem[index+1] = (color >> 8) & 0xff;
      mem[index+2] = (color >> 16) & 0xff;
    }
  }
}

void fill_circle(int x, int y, int r, SDL_Color color) {
  SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
  for (int w = 0; w < r * 2; w++)
  {
    for (int h = 0; h < r * 2; h++)
    {
      int dx = r - w; // horizontal offset
      int dy = r - h; // vertical offset
      if ((dx*dx + dy*dy) <= r*r)
      {
        SDL_RenderDrawPoint(renderer, sdl_x + dx, sdl_y + dy);
      }
    }
  }
}

void render() {
  SDL_GetMouseState(&sdl_x, &sdl_y);
  SDL_RenderClear(renderer);

  // make modifications to surface
  if (SDL_MUSTLOCK(surface)) SDL_LockSurface(surface);
  SDL_FillRect(surface, NULL, 0);
  memcpy(surface->pixels, draw_pixels, WIDTH * HEIGHT * (BPP/8));
  fill_circle2((Uint8*)surface->pixels, sdl_x, sdl_y, 32, 0x88888888);

  // draw with pencil
  if (mouse_button_down != MouseButtonNone) {
    Uint32 col = mouse_button_down == MouseButtonLeft ? 0xffffffff : 0x00000000;
    fill_circle2(draw_pixels, sdl_x, sdl_y, 32, col);
  }

  // draw eisntein
  // SDL_Rect rect = { 0, 0, einstein->w, einstein->h };
  // SDL_BlitSurface(einstein, NULL, surface, &rect);

  if (SDL_MUSTLOCK(surface)) SDL_UnlockSurface(surface);

  // draw surface to screen using renderer
  SDL_Texture* screenTexture = SDL_CreateTextureFromSurface(renderer, surface);

  SDL_RenderCopy(renderer, screenTexture, NULL, NULL);
  SDL_Color col = {255, 255, 255, 255};
  // fill_circle(0, 0, 32, col);
  SDL_RenderPresent(renderer);

  SDL_DestroyTexture(screenTexture);
}

bool mouse_callback(int eventType, const EmscriptenMouseEvent* e, void* userData) {
  if (eventType == EMSCRIPTEN_EVENT_MOUSEDOWN) mouse_button_down = e->button == 0 ? MouseButtonLeft : MouseButtonRight;
  else if (eventType == EMSCRIPTEN_EVENT_MOUSEUP) mouse_button_down = MouseButtonNone;
  return 0;
}

int main(int argc, char** argv) {
  // memset(draw_pixels, 0xff, WIDTH * HEIGHT * (BPP/8));
  
  SDL_Init(SDL_INIT_VIDEO);
  SDL_CreateWindowAndRenderer(WIDTH, HEIGHT, 0, &window, &renderer);
  surface = SDL_CreateRGBSurface(0, WIDTH, HEIGHT, BPP, 0, 0, 0, 0);

  einstein = SDL_LoadBMP("einstein.bmp");

  emscripten_set_mousedown_callback(EMSCRIPTEN_EVENT_TARGET_WINDOW, 0, 1, mouse_callback);
  emscripten_set_mouseup_callback(EMSCRIPTEN_EVENT_TARGET_WINDOW, 0, 1, mouse_callback);
  // emscripten_request_pointerlock(EMSCRIPTEN_EVENT_TARGET_WINDOW, 1);
  emscripten_set_main_loop(render, 0, 1);
}
