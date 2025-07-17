#include <SDL.h>
#include <emscripten.h>
#include <stdlib.h>

const int WIDTH = 300;
const int HEIGHT = 400;
const int BPP = 24;

SDL_Window* window;
SDL_Renderer* renderer;
SDL_Surface* surface;
SDL_Surface* einstein;

void drawRandomPixels() {
  // make modifications to surface
  if (SDL_MUSTLOCK(surface)) SDL_LockSurface(surface);
  SDL_FillRect(surface, NULL, 0); 
  SDL_Rect rect = { 0, 0, einstein->w, einstein->h };
  SDL_BlitSurface(einstein, NULL, surface, &rect);
  if (SDL_MUSTLOCK(surface)) SDL_UnlockSurface(surface);

  // draw surface to screen using renderer
  SDL_Texture* screenTexture = SDL_CreateTextureFromSurface(renderer, surface);

  SDL_RenderClear(renderer);
  SDL_RenderCopy(renderer, screenTexture, NULL, NULL);
  SDL_RenderPresent(renderer);

  SDL_DestroyTexture(screenTexture);
}

int main(int argc, char** argv) {
  SDL_Init(SDL_INIT_VIDEO);
  SDL_CreateWindowAndRenderer(WIDTH, HEIGHT, 0, &window, &renderer);
  surface = SDL_CreateRGBSurface(0, WIDTH, HEIGHT, BPP, 0, 0, 0, 0);

  einstein = SDL_LoadBMP("einstein.bmp");

  emscripten_set_main_loop(drawRandomPixels, 0, 1);
}
