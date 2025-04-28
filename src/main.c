#include <SDL2/SDL.h>
#include <SDL2/SDL_events.h>
#include <SDL2/SDL_keycode.h>
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_video.h>
#include <stdbool.h>
#include <stdio.h>

SDL_Window *window;
SDL_Renderer *renderer;
bool is_running;

bool init_window(void) {
  if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {

    fprintf(stderr, "Error initializing SDL.\n");
    return false;
  }
  window =
      SDL_CreateWindow(NULL, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                       800, 600, SDL_WINDOW_BORDERLESS);
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
  return true;
}

void setup(void) {}
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
void render(void) {
  // Sets a color.
  SDL_SetRenderDrawColor(renderer, 200, 0, 255, 255);
  // Draws that color on backbuffer.
  SDL_RenderClear(renderer);
  // Sends backbuffer to Window.
  SDL_RenderPresent(renderer);
}

int main(void) {
  is_running = init_window();
  setup();
  while (is_running) {
    get_input();
    update();
    render();
  }

  return 0;
}
