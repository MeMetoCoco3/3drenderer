#include "display.h"
#include "vector.h"

bool is_running;
#define N_POINTS (9 * 9 * 9)

vec3_t cube_points[N_POINTS];
vec2_t transformed_points[N_POINTS];

int main(void) {

  printf("BEFORE");
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

void setup(void) {
  // Create color buffer
  color_buffer =
      (uint32_t *)malloc(sizeof(uint32_t) * window_width * window_height);
  // Create texture that will hold color buffer.
  color_buffer_texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_ARGB8888,
                                           SDL_TEXTUREACCESS_STREAMING,
                                           window_width, window_height);
  int point_counter = 0;
  for (float x = -1; x <= 1; x += 0.25) {
    for (float y = -1; y <= 1; y += 0.25) {
      for (float z = -1; z <= 1; z += 0.25) {
        vec3_t new_vector = {.x = x, .y = y, .z = z};
        cube_points[point_counter++] = new_vector;
      }
    }
  }
  printf("$$$%d.\n", point_counter);
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
void update(void) {

  int point_counter = 0;
  for (int i = 0; i < N_POINTS; i++) {
    vec2_t transformed_point = project(cube_points[i]);
    transformed_points[i] = transformed_point;
    point_counter++;
  }

  printf("$$$%d.\n", point_counter);
}

void render(void) {
  // Sets a color.
  /* SDL_SetRenderDrawColor(renderer, 200, 0, 0, 255); */
  // Draws that color on backbuffer.
  /* SDL_RenderClear(renderer); */
  /* draw_rectangle(100, 100, 100, 100, 0xFFFFF000); */
  /* draw_grid(); */
  /* draw_pixel(150, 150, 0xFFFF0000); */

  for (int i = 0; i < N_POINTS; i++) {
    vec2_t point = transformed_points[i];
    draw_rectangle(point.x, point.y, 4, 4, 0xFF0000FF);
  }

  printf("AFTER");
  render_color_buffer();
  clear_color_buffer(0xFF0FFF00);
  // Sends backbuffer to Window.
  SDL_RenderPresent(renderer);
}
