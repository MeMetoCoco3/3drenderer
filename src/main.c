#include "colors.h"
#include "display.h"
#include "mesh.h"
#include "triangle.h"
#include "vector.h"

bool is_running;
int previous_frame_time = 0;

triangle_t triangles_to_render[N_MESH_FACES];

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
  // for (float x = -1; x <= 1; x += 0.25) {
  //   for (float y = -1; y <= 1; y += 0.25) {
  //     for (float z = -1; z <= 1; z += 0.25) {
  //       vec3_t new_vector = {.x = x, .y = y, .z = z};
  //       cube_points[point_counter++] = new_vector;
  //     }
  //   }
  // }
  // printf("$$$%d.\n", point_counter);
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

  cube_rotation.x += 0.02f;
  cube_rotation.y += 0.02f;
  cube_rotation.z += 0.05f;
  for (int i = 0; i < N_MESH_FACES; i++) {
    face_t face = mesh_faces[i];
    vec3_t face_vertices[3];
    face_vertices[0] = mesh_vertices[face.a - 1];
    face_vertices[1] = mesh_vertices[face.b - 1];
    face_vertices[2] = mesh_vertices[face.c - 1];

    triangle_t projected_triangle;

    for (int j = 0; j < 3; j++) {
      vec3_t transformed_vertex = face_vertices[j];

      transformed_vertex = vec3_rotate_x(transformed_vertex, cube_rotation.x);
      transformed_vertex = vec3_rotate_y(transformed_vertex, cube_rotation.y);
      transformed_vertex = vec3_rotate_z(transformed_vertex, cube_rotation.z);

      transformed_vertex.z -= camera_position.z;
      printf("%f\n", transformed_vertex.z);
      vec2_t projected_point = project(transformed_vertex);

      projected_point.x += (window_width / 2);
      projected_point.y += (window_height / 2);

      projected_triangle.points[j] = projected_point;
    }
    triangles_to_render[i] = projected_triangle;
  }
  printf("%f\n", triangles_to_render[0].points[0].x);
}

void render(void) {
  // Sets a color.
  /* SDL_SetRenderDrawColor(renderer, 200, 0, 0, 255); */
  // Draws that color on backbuffer.
  /* SDL_RenderClear(renderer); */
  /* draw_rectangle(100, 100, 100, 100, 0xFFFFF000); */
  /* draw_pixel(150, 150, 0xFFFF0000); */

  draw_grid_points(C_GUNMETAL);

  for (int i = 0; i < N_MESH_FACES; i++) {
    triangle_t triangle = triangles_to_render[i];
    draw_triangle(triangle, C_ORANGE);
    // draw_rectangle(triangle.points[0].x, triangle.points[0].y, 3, 3, C_BLUE);
    // draw_rectangle(triangle.points[1].x, triangle.points[1].y, 3, 3, C_BLUE);
    // draw_rectangle(triangle.points[2].x, triangle.points[2].y, 3, 3, C_BLUE);
  }

  render_color_buffer();
  clear_color_buffer(C_BLACK);
  // Sends backbuffer to Window.
  SDL_RenderPresent(renderer);
}
