// 2025 Taylor Plewe
//
// once emscripten is installed, build with:
//   emcc draw.c -o draw.js \
//   -O3 \
//   -s EXPORT_ES6=1 \
//   -s EXPORT_NAME=draw \
//   -s MODULARIZE=1 \
//   -s USE_SDL=2 \
//   -s EXPORTED_FUNCTIONS='_get_match_percentage,_read_target_pixel_data,_malloc,_free,_main' \
//   -s EXPORTED_RUNTIME_METHODS='cwrap,ccall,HEAPU8'

#include <SDL.h>
#include <emscripten.h>
#include <emscripten/html5.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define PI 3.14159

#define CANVAS_SCALE 3
#define WIDTH 128
#define HEIGHT 128
#define BPP 32
#define COL_BG 0xffffff
#define COL_FG 0x000000
#define PENCIL_MAX_RADIUS 28
#define PENCIL_MIN_RADIUS 0

#define min(a, b) (a < b ? a : b)
#define max(a, b) (a > b ? a : b)

Uint8 draw_pixels[WIDTH * HEIGHT * (BPP/8)];
Uint8* target_pixels;

SDL_Window* window;
SDL_Renderer* renderer;
SDL_Surface* surface;

int pencil_radius = (PENCIL_MAX_RADIUS - PENCIL_MIN_RADIUS) / 2;

typedef enum MouseButton {
  MouseButtonNone,
  MouseButtonLeft,
  MouseButtonRight
} MouseButton;
MouseButton mouse_button_down = MouseButtonNone;

typedef struct Point {
  int x;
  int y;
} Point;
typedef struct PointF {
  float x;
  float y;
} PointF;
Point curr_mouse_point;
Point last_mouse_point;

static inline void color_pixel(Uint8* mem, int x, int y, Uint32 color) {
  int index = ((y * WIDTH) + x) * BPP/8;
  if (index < 0 || index >= WIDTH * HEIGHT * (BPP/8)) return;
  int r =  color        & 0xff;
  int g = (color >> 8)  & 0xff;
  int b = (color >> 16) & 0xff;

  mem[index    ] = r;
  mem[index + 1] = g;
  mem[index + 2] = b;
}

/// Fill in a circle in an array of pixels
void fill_circle(Uint8* mem, int x, int y, int r, Uint32 color) {
  for (int w = 0; w <= r*2; w++) {
    for (int h = 0; h <= r*2; h++) {
      int dx = r - w;
      int dy = r - h;
      int offset_x = (dx + x) % WIDTH;
      int offset_y = dy + y;
      if (
        ((dx*dx) + (dy*dy) > r*r)
        || (offset_x < 0)
        || ((dx + x) >= WIDTH)
      )
        continue;

      color_pixel(mem, offset_x, offset_y, color);
    }
  }
}

void fill_segment(Uint8* mem, Point p1, Point p2, int r, Uint32 color) {
  last_mouse_point = curr_mouse_point;

  float theta = SDL_atan2f(p2.y - p1.y, p2.x - p1.x);

  if (r > 0) {
    // fill in a rotated rectangle with a thickness of r from p1 to p2
    float  theta_perp = theta + PI/2;
    float  x_offs     = r * SDL_cosf(theta_perp);
    float  y_offs     = r * SDL_sinf(theta_perp);
    PointF a          = { p2.x + x_offs, p2.y + y_offs };
    PointF b          = { p1.x + x_offs, p1.y + y_offs };
    PointF c          = { p2.x - x_offs, p2.y - y_offs };
    PointF d          = { p1.x - x_offs, p1.y - y_offs };
    PointF ab         = { b.x - a.x, b.y - a.y };
    PointF ac         = { c.x - a.x, c.y - a.y };
    float  len1_sq    = (ab.x*ab.x) + (ab.y*ab.y);
    float  len2_sq    = (ac.x*ac.x) + (ac.y*ac.y);

    int bound_top    = max(min(min(min(a.y, b.y), c.y), d.y), 0);
    int bound_left   = max(min(min(min(a.x, b.x), c.x), d.x), 0);
    int bound_bottom = min(max(max(max(a.y, b.y), c.y), d.y), HEIGHT - 1);
    int bound_right  = min(max(max(max(a.x, b.x), c.x), d.x), WIDTH - 1);

    for (int row = bound_top; row <= bound_bottom; row++) {
      for (int col = bound_left; col <= bound_right; col++) {
        Point p = { col, row };
        PointF ap = { p.x - a.x, p.y - a.y };
        float dot1 = ab.x * ap.x + ab.y * ap.y;
        float dot2 = ac.x * ap.x + ac.y * ap.y;

        if (0 <= dot1 && dot1 <= len1_sq && 0 <= dot2 && dot2 <= len2_sq) {
          color_pixel(mem, col, row, color);
        }
      }
    }
  } else {
    // draw a 1px-thick line from p1 to p2
    float x_offs = SDL_cosf(theta);
    float y_offs = SDL_sinf(theta);

    Point curr_pixel = { p1.x, p1.y };
    PointF virtual_draw_point = { p1.x + 0.5f, p1.y + 0.5f };

    while (curr_pixel.x != p2.x || curr_pixel.y != p2.y) {
      // update virtual 2D point in line by one unit
      virtual_draw_point.x += x_offs;
      virtual_draw_point.y += y_offs;
      
      // update the integer version
      curr_pixel.x = SDL_floorf(virtual_draw_point.x);
      curr_pixel.y = SDL_floorf(virtual_draw_point.y);

      // fill in that pixel
      color_pixel(mem, curr_pixel.x, curr_pixel.y, color);
    }
  }
}

// mouse event handlers
bool mouse_callback(int eventType, const EmscriptenMouseEvent* e, void* userData) {
  if (eventType == EMSCRIPTEN_EVENT_MOUSEDOWN) {
    last_mouse_point = curr_mouse_point;
    mouse_button_down = e->button == 0
      ? MouseButtonLeft
      : MouseButtonRight;
  }
  else if (eventType == EMSCRIPTEN_EVENT_MOUSEUP) mouse_button_down = MouseButtonNone;
  else if (eventType == EMSCRIPTEN_EVENT_MOUSEMOVE) {
    curr_mouse_point.x = e->targetX / CANVAS_SCALE;
    curr_mouse_point.y = e->targetY / CANVAS_SCALE;
  }
  return 0;
}
bool wheel_callback(int eventType, const EmscriptenWheelEvent* e, void* userData) {
  pencil_radius += (int)(e->deltaY/2);
  if (pencil_radius > PENCIL_MAX_RADIUS) pencil_radius = PENCIL_MAX_RADIUS;
  if (pencil_radius < PENCIL_MIN_RADIUS) pencil_radius = PENCIL_MIN_RADIUS;
  return 0;
}

void update() {
  // pencil update logic
  if (mouse_button_down != MouseButtonNone) {
    Uint32 col = mouse_button_down == MouseButtonLeft ? COL_FG : COL_BG;
    fill_segment(draw_pixels, last_mouse_point, curr_mouse_point, pencil_radius, col);
    fill_circle(draw_pixels, curr_mouse_point.x, curr_mouse_point.y, pencil_radius, col);
    // mouse_button_down = MouseButtonNone; // debug
  }

  SDL_RenderClear(renderer);

  // make modifications to surface; lock for thread safety
  if (SDL_MUSTLOCK(surface)) SDL_LockSurface(surface);

  // first, draw everything the user has drawn thus far
  memcpy(surface->pixels, draw_pixels, WIDTH * HEIGHT * (BPP/8));

  // second, draw the user's cursor on top of that
  fill_circle((Uint8*)surface->pixels, curr_mouse_point.x, curr_mouse_point.y, pencil_radius, 0x88888888);

  if (SDL_MUSTLOCK(surface)) SDL_UnlockSurface(surface);

  // draw surface to screen using renderer & wrap up frame
  SDL_Texture* screenTexture = SDL_CreateTextureFromSurface(renderer, surface);
  SDL_RenderCopy(renderer, screenTexture, NULL, NULL);
  SDL_RenderPresent(renderer);
  SDL_DestroyTexture(screenTexture);
}

int main(int argc, char** argv) {
  memset(draw_pixels, COL_BG & 0xff, WIDTH * HEIGHT * (BPP/8));
  
  SDL_Init(SDL_INIT_VIDEO);
  SDL_CreateWindowAndRenderer(WIDTH, HEIGHT, 0, &window, &renderer);
  surface = SDL_CreateRGBSurface(0, WIDTH, HEIGHT, BPP, 0, 0, 0, 0);

  emscripten_set_mousedown_callback(EMSCRIPTEN_EVENT_TARGET_WINDOW, 0, 1, mouse_callback);
  emscripten_set_mouseup_callback(EMSCRIPTEN_EVENT_TARGET_WINDOW, 0, 1, mouse_callback);
  emscripten_set_mousemove_callback("canvas#drawable", 0, 1, mouse_callback);
  emscripten_set_wheel_callback(EMSCRIPTEN_EVENT_TARGET_WINDOW, 0, 1, wheel_callback);
  emscripten_set_main_loop(update, 0, 1);
}

// exported functions
float match_percentage = 0.0f;
EMSCRIPTEN_KEEPALIVE
float get_match_percentage() {
  int num_matched = 0;
  for (int i = 0; i < WIDTH * HEIGHT * (BPP/8); i += BPP/8) {
    if (
      draw_pixels[i] == target_pixels[i]
      && draw_pixels[i+1] == target_pixels[i+1]
      && draw_pixels[i+2] == target_pixels[i+2]
      // do not compare alpha channel; they will always match
    ) num_matched++;
  }
  return (float)num_matched / (WIDTH * HEIGHT);
}

EMSCRIPTEN_KEEPALIVE
void read_target_pixel_data(Uint8* data, int data_length) {
  target_pixels = data;
}
