#include "array.h"
#include "colors.h"
#include "display.h"
#include "matrix.h"
#include "mesh.h"
#include "triangle.h"
#include "vector.h"
bool is_running;
int previous_frame_time = 0;

triangle_t *triangles_to_render = NULL;

rendering_data_t rendering_data;

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

  rendering_data.rm = RM_WIREFRAME;
  rendering_data.bc = BACKFACE_CULLING_ON;
  // Create color buffer
  color_buffer =
      (uint32_t *)malloc(sizeof(uint32_t) * window_width * window_height);
  // Create texture that will hold color buffer.
  color_buffer_texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_ARGB8888,
                                           SDL_TEXTUREACCESS_STREAMING,
                                           window_width, window_height);
  // load_obj_file_data("./assets/cube.obj");
  load_cube_mesh_data();
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
    if (event.key.keysym.sym == SDLK_KP_1) {
      rendering_data.rm = RM_WIREFRAME;
    }
    if (event.key.keysym.sym == SDLK_KP_2) {
      rendering_data.rm = RM_WIREFRAME_LINES;
    }
    if (event.key.keysym.sym == SDLK_KP_3) {
      rendering_data.rm = RM_COLORED;
    }
    if (event.key.keysym.sym == SDLK_KP_4) {
      rendering_data.rm = RM_COLORED_LINES;
    }
    if (event.key.keysym.sym == SDLK_c) {
      rendering_data.bc = BACKFACE_CULLING_ON;
    }
    if (event.key.keysym.sym == SDLK_d) {
      rendering_data.bc = BACKFACE_CULLING_OF;
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

  mesh.rotation.x += 0.01f;
  mesh.rotation.y += 0.01f;
  mesh.rotation.z += 0.02f;

  // mesh.scale.x += 0.001f;
  // mesh.scale.y += 0.001f;

  mesh.translation.x += 0.01f;
  mesh.translation.z = depth;

  mat4_t scale_matrix =
      mat4_make_scale(mesh.scale.x, mesh.scale.y, mesh.scale.z);
  mat4_t translation_matrix = mat4_make_translation(
      mesh.translation.x, mesh.translation.y, mesh.translation.z);
  mat4_t rotation_matrix_x = mat4_make_rotation_x(mesh.rotation.x);
  mat4_t rotation_matrix_y = mat4_make_rotation_y(mesh.rotation.y);
  mat4_t rotation_matrix_z = mat4_make_rotation_z(mesh.rotation.z);

  int num_faces = array_length(mesh.faces);
  for (int i = 0; i < num_faces; i++) {
    face_t face = mesh.faces[i];
    vec3_t face_vertices[3];
    face_vertices[0] = mesh.vertices[face.a - 1];
    face_vertices[1] = mesh.vertices[face.b - 1];
    face_vertices[2] = mesh.vertices[face.c - 1];

    vec4_t transformed_vertices[3];

    // Transformation
    for (int j = 0; j < 3; j++) {
      vec4_t transformed_vertex = vec4_from_vec3(face_vertices[j]);
      transformed_vertex = mat4_mul_vec4(scale_matrix, transformed_vertex);

      transformed_vertex = mat4_mul_vec4(rotation_matrix_z, transformed_vertex);
      transformed_vertex = mat4_mul_vec4(rotation_matrix_y, transformed_vertex);
      transformed_vertex = mat4_mul_vec4(rotation_matrix_x, transformed_vertex);
      transformed_vertex =
          mat4_mul_vec4(translation_matrix, transformed_vertex);

      transformed_vertices[j] = transformed_vertex;
    }

    // Backface culling
    if (rendering_data.bc == BACKFACE_CULLING_ON) {
      vec3_t vector_a = vec3_from_vec4(transformed_vertices[0]);
      vec3_t vector_b = vec3_from_vec4(transformed_vertices[1]);
      vec3_t vector_c = vec3_from_vec4(transformed_vertices[2]);

      vec3_t vector_ba = vec3_sub(vector_b, vector_a);
      vec3_t vector_ca = vec3_sub(vector_c, vector_a);
      vec3_normalize(&vector_ba);
      vec3_normalize(&vector_ca);

      vec3_t normal = vec3_cross(vector_ba, vector_ca);
      vec3_normalize(&normal);

      vec3_t camera_ray = vec3_sub(camera_position, vector_a);

      float angle = vec3_dot(normal, camera_ray);

      if (angle < 0.0) {
        continue;
      }
    }
    // Projection
    vec2_t projected_points[3];
    for (int j = 0; j < 3; j++) {
      vec2_t projected_point = project(vec3_from_vec4(transformed_vertices[j]));

      projected_point.x += (window_width / 2);
      projected_point.y += (window_height / 2);

      projected_points[j] = projected_point;
    }
    float avg_depth = (transformed_vertices[0].z + transformed_vertices[1].z +
                       transformed_vertices[2].z) /
                      3;

    triangle_t projected_triangle = {
        .points =
            {
                {projected_points[0].x, projected_points[0].y},
                {projected_points[1].x, projected_points[1].y},
                {projected_points[2].x, projected_points[2].y},
            },
        .color = mesh.faces[i].color,
        .avg_depth = avg_depth,
    };

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
    if (rendering_data.rm == RM_COLORED ||
        rendering_data.rm == RM_COLORED_LINES) {

      draw_filled_triangle(triangle.points[0].x, triangle.points[0].y,
                           triangle.points[1].x, triangle.points[1].y,
                           triangle.points[2].x, triangle.points[2].y,
                           triangle.color);
    }
    if (rendering_data.rm == RM_COLORED_LINES ||
        rendering_data.rm == RM_WIREFRAME ||
        rendering_data.rm == RM_WIREFRAME_LINES) {
      draw_triangle(triangle, C_BLUE);
    }
    if (rendering_data.rm == RM_WIREFRAME) {
      draw_rectangle(triangle.points[0].x, triangle.points[0].y, 3, 3, C_RED);
      draw_rectangle(triangle.points[1].x, triangle.points[1].y, 3, 3, C_RED);
      draw_rectangle(triangle.points[2].x, triangle.points[2].y, 3, 3, C_RED);
    }
  }

  array_free(triangles_to_render);

  render_color_buffer();
  clear_color_buffer(C_BLACK);
  // Sends backbuffer to Window.
  SDL_RenderPresent(renderer);
}
