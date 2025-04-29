#include <SDL2/SDL.h>
#include <SDL2/SDL_events.h>
#include <SDL2/SDL_keycode.h>
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_video.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>

bool is_running;

SDL_Window *window;
SDL_Renderer *renderer;

uint32_t *color_buffer;
SDL_Texture *color_buffer_texture;

int window_height = 600;
int window_width = 800;

bool init_window(void) {
  if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {

    fprintf(stderr, "Error initializing SDL.\n");
    return false;
  }

  // Query SDL to know size of display.
  SDL_DisplayMode display_mode;
  SDL_GetCurrentDisplayMode(0, &display_mode);
  window_height = display_mode.h;
  window_width = display_mode.w;

  window =
      SDL_CreateWindow(NULL, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                       window_width, window_height, SDL_WINDOW_BORDERLESS);
  if (!window) {
    fprintf(stderr, "Error initializing Window.\n");
    return false;
  }
  // El segundo argumento define el device context, si es -1, pilla el primero
  // que pueda.
  renderer = SDL_CreateRenderer(window, -1, 0);
  if (!renderer) {
    fprintf(stderr, "Error initializing Renderer.\n");
    return false;
  }
  // Set Fullscreen.
  SDL_SetWindowFullscreen(window, SDL_WINDOW_FULLSCREEN);
  return true;
}

void setup(void) {
  color_buffer =
      (uint32_t *)malloc(sizeof(uint32_t) * window_width * window_height);
  color_buffer_texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_ARGB8888,
                                           SDL_TEXTUREACCESS_STREAMING,
                                           window_width, window_height);
}
/* FROM EXERCICE DRAWING A BACKGROUND GRID*/
void draw_grid(void) {
  for (int y = 0; y < window_height; y++) {
    for (int x = 0; x < window_width; x++)
      if (y % 10 == 0 || x % 10 == 0) {
        color_buffer[y * window_width + x] = 0xFF000000;
      }
  }
}

void get_input(void) {
  SDL_Event event;
  SDL_PollEvent(&event);
  switch (event.type) {
  case SDL_QUIT:
    is_running = false;
    break;
  case SDL_KEYDOWN:
    if (event.key.keysym.sym == SDLK_ESCAPE) {
      is_running = false;
    }
    break;
  }
}
void update(void) {}

void render_color_buffer(void) {
  // Updates texture with the values from color buffer.
  SDL_UpdateTexture(color_buffer_texture, NULL, color_buffer,
                    (int)sizeof(uint32_t) * window_width);
  // Renders texture on device.
  SDL_RenderCopy(renderer, color_buffer_texture, NULL, NULL);
}
void clear_color_buffer(uint32_t color) {
  for (int y = 0; y < window_height; y++) {
    for (int x = 0; x < window_width; x++)
      color_buffer[y * window_width + x] = color;
  }
}

void render(void) {
  // Sets a color.
  SDL_SetRenderDrawColor(renderer, 200, 0, 0, 255);
  // Draws that color on backbuffer.
  SDL_RenderClear(renderer);
  render_color_buffer();
  clear_color_buffer(0xFFFFFF00);
  draw_grid();
  // Sends backbuffer to Window.
  SDL_RenderPresent(renderer);
}

void destroy_window(void) {
  free(color_buffer);
  printf("Color buffer freed.\n");

  SDL_DestroyRenderer(renderer);
  printf("Renderer freed.\n");

  SDL_DestroyWindow(window);
  printf("Window freed.\n");

  SDL_Quit();
  printf("SDL QUIT.\n");
}

int main(void) {
  is_running = init_window();
  setup();
  while (is_running) {
    get_input();
    update();
    render();
  }
  destroy_window();
  return 0;
}
