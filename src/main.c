#include "display.h"
#include "vector.h"

bool is_running;
int previous_frame_time = 0;

#define N_POINTS (9 * 9 * 9)
vec3_t cube_points[N_POINTS];
vec2_t transformed_points[N_POINTS];

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
  // Deltatime
  int time_to_wait = FRAME_TARGET_TIME - (SDL_GetTicks() - previous_frame_time);
  if (time_to_wait > 0 && time_to_wait <= FRAME_TARGET_TIME) {
    SDL_Delay(time_to_wait);
  }
  previous_frame_time = SDL_GetTicks();

  cube_rotation.y += 0.001f;
  cube_rotation.x += 0.01f;

  for (int i = 0; i < N_POINTS; i++) {
    vec3_t point = cube_points[i];

    vec3_t rotated_point = vec3_rotate_y(point, cube_rotation.y);
    rotated_point = vec3_rotate_x(rotated_point, cube_rotation.x);

    rotated_point.z -= camera_position.z;

    vec2_t transformed_point = project(rotated_point);
    transformed_point.x += window_width / 2.0f;
    transformed_point.y += window_height / 2.0f;
    transformed_points[i] = transformed_point;
  }
}

void render(void) {
  // Sets a color.
  /* SDL_SetRenderDrawColor(renderer, 200, 0, 0, 255); */
  // Draws that color on backbuffer.
  /* SDL_RenderClear(renderer); */
  /* draw_rectangle(100, 100, 100, 100, 0xFFFFF000); */
  /* draw_pixel(150, 150, 0xFFFF0000); */

  for (int i = 0; i < N_POINTS; i++) {
    // Colours based on depth.
    /* int z_value = (cube_points[i].z + 1.0f) * 100; */
    /* uint32_t color = (255 << 24) | (z_value << 16) | (z_value << 8) | 255; */

    vec2_t point = transformed_points[i];
    draw_rectangle(point.x, point.y, 4, 4, 0x000000FF);
  }

  draw_grid_points(0xFF000000);
  render_color_buffer();
  clear_color_buffer(0xFFFFFFFF);
  // Sends backbuffer to Window.
  SDL_RenderPresent(renderer);
}
