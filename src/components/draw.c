#include <SDL.h>
#include <emscripten.h>
#include <emscripten/html5.h>
#include <stdlib.h>

const int WIDTH = 300;
const int HEIGHT = 400;
const int BPP = 24;

SDL_Window* window;
SDL_Renderer* renderer;
SDL_Surface* surface;
SDL_Surface* einstein;

int mouse_x = 0;
int mouse_y = 0;
int sdl_x, sdl_y;

static inline const char *emscripten_event_type_to_string(int eventType) {
  const char *events[] = { "(invalid)", "(none)", "keypress", "keydown", "keyup", "click", "mousedown", "mouseup", "dblclick", "mousemove", "wheel", "resize", 
    "scroll", "blur", "focus", "focusin", "focusout", "deviceorientation", "devicemotion", "orientationchange", "fullscreenchange", "pointerlockchange", 
    "visibilitychange", "touchstart", "touchend", "touchmove", "touchcancel", "gamepadconnected", "gamepaddisconnected", "beforeunload", 
    "batterychargingchange", "batterylevelchange", "webglcontextlost", "webglcontextrestored", "(invalid)" };
  ++eventType;
  if (eventType < 0) eventType = 0;
  if (eventType >= sizeof(events)/sizeof(events[0])) eventType = sizeof(events)/sizeof(events[0])-1;
  return events[eventType];
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
  // SDL_Rect rect = { 0, 0, einstein->w, einstein->h };
  // SDL_BlitSurface(einstein, NULL, surface, &rect);
  if (SDL_MUSTLOCK(surface)) SDL_UnlockSurface(surface);

  // draw surface to screen using renderer
  SDL_Texture* screenTexture = SDL_CreateTextureFromSurface(renderer, surface);

  SDL_RenderCopy(renderer, screenTexture, NULL, NULL);
  SDL_Color col = {255, 255, 255, 255};
  fill_circle(0, 0, 32, col);
  SDL_RenderPresent(renderer);

  SDL_DestroyTexture(screenTexture);
}

bool mouse_callback(int eventType, const EmscriptenMouseEvent* e, void *userData) {
  printf("%s\n target: (%d, %d)\n canvas: (%d, %d)\n client: (%d, %d)\n\n", emscripten_event_type_to_string(eventType), e->targetX, e->targetY, e->canvasX, e->canvasY, e->clientX, e->clientY);
  return 0;
}

int main(int argc, char** argv) {
  SDL_Init(SDL_INIT_VIDEO);
  SDL_CreateWindowAndRenderer(WIDTH, HEIGHT, 0, &window, &renderer);
  surface = SDL_CreateRGBSurface(0, WIDTH, HEIGHT, BPP, 0, 0, 0, 0);

  einstein = SDL_LoadBMP("einstein.bmp");

  emscripten_set_mousedown_callback(EMSCRIPTEN_EVENT_TARGET_WINDOW, 0, 1, mouse_callback);
  emscripten_set_mouseup_callback(EMSCRIPTEN_EVENT_TARGET_WINDOW, 0, 1, mouse_callback);
  emscripten_set_main_loop(render, 0, 1);
}
