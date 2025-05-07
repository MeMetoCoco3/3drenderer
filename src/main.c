#include "array.h"
#include "colors.h"
#include "display.h"
#include "mesh.h"
#include "triangle.h"
#include "vector.h"

bool is_running;
int previous_frame_time = 0;

triangle_t *triangles_to_render = NULL;

int main(void) {
  is_running = init_window();

  setup();

  while (is_running) {
    get_input();
    update();
    render();
  }
  destroy_window();
  free_resources();
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
  load_obj_file_data("./assets/pig.obj");
  // load_cube_mesh_data();
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

  triangles_to_render = NULL;

  mesh.rotation.x += 0.02f;
  mesh.rotation.y += 0.02f;
  mesh.rotation.z += 0.05f;

  int num_faces = array_length(mesh.faces);
  for (int i = 0; i < num_faces; i++) {
    face_t face = mesh.faces[i];
    vec3_t face_vertices[3];
    face_vertices[0] = mesh.vertices[face.a - 1];
    face_vertices[1] = mesh.vertices[face.b - 1];
    face_vertices[2] = mesh.vertices[face.c - 1];

    triangle_t projected_triangle;

    vec3_t transformed_vertices[3];

    // Transformation
    for (int j = 0; j < 3; j++) {
      vec3_t transformed_vertex = face_vertices[j];

      transformed_vertex = vec3_rotate_x(transformed_vertex, mesh.rotation.x);
      transformed_vertex = vec3_rotate_y(transformed_vertex, mesh.rotation.y);
      transformed_vertex = vec3_rotate_z(transformed_vertex, mesh.rotation.z);

      transformed_vertex.z += depth;
      transformed_vertices[j] = transformed_vertex;
    }
    // Backface culling
    vec3_t vector_a = transformed_vertices[0];
    vec3_t vector_b = transformed_vertices[1];
    vec3_t vector_c = transformed_vertices[2];

    vec3_t vector_ba = vec3_sub(vector_b, vector_a);
    vec3_t vector_ca = vec3_sub(vector_c, vector_a);

    vec3_t normal = vec3_cross(vector_ba, vector_ca);

    vec3_t camera_ray = vec3_sub(camera_position, vector_a);

    float angle = vec3_dot(normal, camera_ray);

    if (angle < 0.0) {
      continue;
    }

    // Projection
    for (int j = 0; j < 3; j++) {
      vec2_t projected_point = project(transformed_vertices[j]);

      projected_point.x += (window_width / 2);
      projected_point.y += (window_height / 2);

      projected_triangle.points[j] = projected_point;
    }
    array_push(triangles_to_render, projected_triangle);
  }
}

void render(void) {
  // Sets a color.
  /* SDL_SetRenderDrawColor(renderer, 200, 0, 0, 255); */
  // Draws that color on backbuffer.
  /* SDL_RenderClear(renderer); */
  /* draw_rectangle(100, 100, 100, 100, 0xFFFFF000); */
  /* draw_pixel(150, 150, 0xFFFF0000); */

  draw_grid_points(C_GUNMETAL);

  int num_triangles = array_length(triangles_to_render);
  for (int i = 0; i < num_triangles; i++) {
    triangle_t triangle = triangles_to_render[i];
    draw_triangle(triangle, C_ORANGE);
    draw_rectangle(triangle.points[0].x, triangle.points[0].y, 3, 3, C_RED);
    draw_rectangle(triangle.points[1].x, triangle.points[1].y, 3, 3, C_RED);
    draw_rectangle(triangle.points[2].x, triangle.points[2].y, 3, 3, C_RED);
  }

  array_free(triangles_to_render);

  render_color_buffer();
  clear_color_buffer(C_BLACK);
  // Sends backbuffer to Window.
  SDL_RenderPresent(renderer);
}
