#include "main.h"
#include "array.h"
#include "camera.h"
#include "clipping.h"
#include "colors.h"
#include "display.h"
#include "light.h"
#include "matrix.h"
#include "sort.h"
#include "triangle.h"
#include "upng.h"
#include "vector.h"
#include <math.h>

#define MAX_TRIANGLES_PER_MESH 10000
triangle_t triangles_to_render[MAX_TRIANGLES_PER_MESH];
int num_triangles_to_render;

bool is_running;
int previous_frame_time = 0;
float delta_time = 0.0;
mat4_t projection_matrix;
mat4_t view_matrix;
mat4_t world_matrix;

int main(void) {
  is_running = init_window();

  setup();

  while (is_running) {
    get_input();
    update();
    render();
  }
  free_resources();
  return 0;
}

void setup(void) {
  // Aplicamos default values de como queremos renderizar nuestros modelos.
  set_render_method(RM_COLORED_LINES);
  set_cull_method(BACKFACE_CULLING_ON);
  set_light(LIGHTS_OF);

  init_light(vec3_new(0, 0, -1));

  // Matrix Projection.
  // FOV en radianes (equivalente a 60 grados).
  float aspect_y = (float)get_window_height() / (float)get_window_width();
  float FOV_Y = 3.14159 / 3.0;
  float aspect_x = (float)get_window_width() / (float)get_window_height();
  float FOV_X = 2 * (atan(tan(FOV_Y / 2) * aspect_x));
  float znear = 0.1;
  float zfar = 100.0;
  projection_matrix = mat4_make_perspective(FOV_Y, aspect_y, znear, zfar);

  init_frustum_planes(FOV_X, FOV_Y, znear, zfar);

  // Estas dos funciones se encargan de:
  // Cargar objetos en nuestra mesh.
  load_mesh("./assets/crab.obj", "./assets/crab.png", vec3_new(1, 1, 1),
            vec3_new(-1.3, 0, +8), vec3_new(0, 0, 0));

  load_mesh("./assets/heart.obj", "./assets/crab.png", vec3_new(1, 1, 1),
            vec3_new(1.3, 0, +8), vec3_new(0, 2.6, 0));
}

void get_input(void) {
  // Estructura encargada de lidiar con todos los eventos de nuestro SO.
  // Utilizado para lidiar con keyboard input.
  SDL_Event event;
  while (SDL_PollEvent(&event)) {
    switch (event.type) {
    case SDL_QUIT:
      is_running = false;
      break;
    case SDL_KEYDOWN:
      if (event.key.keysym.sym == SDLK_ESCAPE) {
        is_running = false;
        break;
      }
      if (event.key.keysym.sym == SDLK_KP_1) {
        set_render_method(RM_WIREFRAME);
        break;
      }
      if (event.key.keysym.sym == SDLK_KP_2) {
        set_render_method(RM_WIREFRAME_VERTEX);
        break;
      }
      if (event.key.keysym.sym == SDLK_KP_3) {
        set_render_method(RM_COLORED);
        break;
      }
      if (event.key.keysym.sym == SDLK_KP_4) {
        set_render_method(RM_COLORED_LINES);
        break;
      }
      if (event.key.keysym.sym == SDLK_KP_5) {
        set_render_method(RM_TEXTURED);
        break;
      }
      if (event.key.keysym.sym == SDLK_KP_6) {
        set_render_method(RM_TEXTURED_WIRE);
        break;
      }
      if (event.key.keysym.sym == SDLK_c) {
        set_cull_method(BACKFACE_CULLING_ON);
        break;
      }
      if (event.key.keysym.sym == SDLK_b) {
        set_light(LIGHTS_OF);
        set_cull_method(BACKFACE_CULLING_OF);
        break;
      }
      if (event.key.keysym.sym == SDLK_k) {
        set_cull_method(BACKFACE_CULLING_ON);
        set_light(LIGHTS_ON);
        break;
      }

      if (event.key.keysym.sym == SDLK_UP) {
        set_camera_forward_velocity(
            vec3_mul(get_camera_direction(), 5.0 * delta_time));
        set_camera_position(
            vec3_add(get_camera_position(), get_camera_forward_velocity()));
        break;
      }
      if (event.key.keysym.sym == SDLK_DOWN) {
        set_camera_forward_velocity(
            vec3_mul(get_camera_direction(), 5.0 * delta_time));
        set_camera_position(
            vec3_sub(get_camera_position(), get_camera_forward_velocity()));
        break;
      }
      if (event.key.keysym.sym == SDLK_a) {
        rotate_camera_yaw(+1.0 * delta_time);
        break;
      }
      if (event.key.keysym.sym == SDLK_d) {
        rotate_camera_yaw(-1.0 * delta_time);
        break;
      }
      if (event.key.keysym.sym == SDLK_w) {
        rotate_camera_pitch(+1.0 * delta_time);
        break;
      }
      if (event.key.keysym.sym == SDLK_s) {
        rotate_camera_pitch(-1.0 * delta_time);
        break;
      }
      break;
    }
  }
}

void process_graphics_pipeline_stages(mesh_t *mesh) {
  // mesh.rotation.y += 0.02f;
  // mesh.scale.y += 0.001f;

  // Me estaba rayando mucho con esto ya que pensaba, si la camara gira,
  // porque coño tengo que calcular la rotacion de la target, se aplica la yaw
  // y au, y eso se tendria en cuenta en la look at function. Pero no, el
  // origen es la camara, pero el eje de coordenadas no cambia, por ello
  // debemos rotar.

  // El orden importa, siempre aplicaremos antes:
  // 1. Scale.
  // 2. Rotation z,y,x.
  // 3. Translation.
  // 0. Sheer debe ser aplicado antes o despues que scale.
  mat4_t scale_matrix =
      mat4_make_scale(mesh->scale.x, mesh->scale.y, mesh->scale.z);
  mat4_t translation_matrix = mat4_make_translation(
      mesh->translation.x, mesh->translation.y, mesh->translation.z);
  mat4_t rotation_matrix_x = mat4_make_rotation_x(mesh->rotation.x);
  mat4_t rotation_matrix_y = mat4_make_rotation_y(mesh->rotation.y);
  mat4_t rotation_matrix_z = mat4_make_rotation_z(mesh->rotation.z);

  vec3_t target = get_camera_lookat_target();
  vec3_t up_direction = vec3_new(0, 1, 0);
  view_matrix = mat4_look_at(get_camera_position(), target, up_direction);

  int num_faces = array_length(mesh->faces);
  for (int i = 0; i < num_faces; i++) {
    face_t face = mesh->faces[i];
    vec3_t face_vertices[3];
    // TODO:REPASAR ESTA INDEXACION YA QUE LAS FACES SE SUELEN HACER CON
    // INDICE 1
    face_vertices[0] = mesh->vertices[face.a];
    face_vertices[1] = mesh->vertices[face.b];
    face_vertices[2] = mesh->vertices[face.c];

    vec4_t transformed_vertices[3];
    for (int j = 0; j < 3; j++) {
      // Creamos una matriz mundo, le aplicamos todas las matrices declaradas
      // anteriormente. Y despues aplicaremos el resultado a los vertices.
      vec4_t transformed_vertex = vec4_from_vec3(face_vertices[j]);

      world_matrix = mat4_identity();

      world_matrix = mat4_mul_mat4(scale_matrix, world_matrix);
      world_matrix = mat4_mul_mat4(rotation_matrix_z, world_matrix);
      world_matrix = mat4_mul_mat4(rotation_matrix_y, world_matrix);
      world_matrix = mat4_mul_mat4(rotation_matrix_x, world_matrix);
      world_matrix = mat4_mul_mat4(translation_matrix, world_matrix);

      transformed_vertex = mat4_mul_vec4(world_matrix, transformed_vertex);
      transformed_vertex = mat4_mul_vec4(view_matrix, transformed_vertex);
      transformed_vertices[j] = transformed_vertex;
    }

    vec3_t face_normal = get_triangle_normal(transformed_vertices);
    // Backface culling
    if (is_cull_backface()) {

      vec3_t origen = {0, 0, 0};
      vec3_t camera_ray =
          vec3_sub(vec3_new(0, 0, 0), vec3_from_vec4(transformed_vertices[0]));

      // El dot product nos da el angulo entre dos vectores.
      float angle = vec3_dot(face_normal, camera_ray);

      // Si el angulo es negativo o 0, no dibujaremos esa cara.
      if (angle < 0.0) {
        continue;
      }
    }

    // Clipping!!
    polygon_t polygon =
        create_polygon_from_triangle(vec3_from_vec4(transformed_vertices[0]),
                                     vec3_from_vec4(transformed_vertices[1]),
                                     vec3_from_vec4(transformed_vertices[2]),
                                     face.a_uv, face.b_uv, face.c_uv);
    clip_polygon(&polygon);

    triangle_t triangles_after_clipping[MAX_NUM_POLYGON_TRIANGLES];
    int num_triangles_after_clipping = 0;

    triangles_from_polygon(&polygon, triangles_after_clipping,
                           &num_triangles_after_clipping);

    for (int t = 0; t < num_triangles_after_clipping; t++) {
      triangle_t triangle_after_clipping = triangles_after_clipping[t];
      // Projection
      vec4_t projected_points[3];
      for (int j = 0; j < 3; j++) {
        // Multiplicamos los vectores por la matriz de projeccion.
        // En la funcion aplicamos perspective divide.
        // El resultado esta en un rango de -1 a 1 en x, y, z, llamado
        // NDC(Normalized Device Coordinates).
        projected_points[j] = mat4_mul_vec4_project(
            projection_matrix, triangle_after_clipping.points[j]);

        projected_points[j].y *= -1;
        // Escalar a Screen Coordinates.
        projected_points[j].x *= (get_window_width() / 2.0);
        projected_points[j].y *= (get_window_height() / 2.0);

        // Trasladar a Screen Coordinates.
        projected_points[j].x += (get_window_width() / 2.0);
        projected_points[j].y += (get_window_height() / 2.0);
      }

      uint32_t final_color = mesh->faces[i].color;
      if (is_lights_on()) {
        vec3_normalize(&face_normal);

        vec3_t light_dir = get_light_direction();
        vec3_normalize(&light_dir);

        float light_intensity_factor = vec3_dot(face_normal, light_dir);

        light_intensity_factor = clamp_f(light_intensity_factor, 0.2f, 1.0f);

        final_color =
            light_apply_intensity(mesh->faces[i].color, light_intensity_factor);
      }

      triangle_t triangle_to_render = {
          .points = {{projected_points[0].x, projected_points[0].y,
                      projected_points[0].z, projected_points[0].w},
                     {projected_points[1].x, projected_points[1].y,
                      projected_points[1].z, projected_points[1].w},
                     {projected_points[2].x, projected_points[2].y,
                      projected_points[2].z, projected_points[2].w}},
          .textcoords = {{triangle_after_clipping.textcoords[0].u,
                          triangle_after_clipping.textcoords[0].v},
                         {triangle_after_clipping.textcoords[1].u,
                          triangle_after_clipping.textcoords[1].v},
                         {triangle_after_clipping.textcoords[2].u,
                          triangle_after_clipping.textcoords[2].v}},
          .color = final_color,
          .texture = mesh->texture};

      if (num_triangles_to_render < MAX_TRIANGLES_PER_MESH) {
        triangles_to_render[num_triangles_to_render] = triangle_to_render;
        num_triangles_to_render++;
      }
    }
  }
}

void update(void) {

  int time_to_wait = FRAME_TARGET_TIME - (SDL_GetTicks() - previous_frame_time);
  if (time_to_wait > 0 && time_to_wait <= FRAME_TARGET_TIME) {
    SDL_Delay(time_to_wait);
  }

  delta_time = (SDL_GetTicks() - previous_frame_time) / 1000.0;
  previous_frame_time = SDL_GetTicks();

  num_triangles_to_render = 0;
  for (int mesh_index = 0; mesh_index < get_num_meshes(); mesh_index++) {
    mesh_t *mesh = get_mesh(mesh_index);
    process_graphics_pipeline_stages(mesh);
  }
}

void render(void) {
  // SDL_RenderClear(renderer);

  // Limpiamos el color buffer.
  clear_color_buffer(C_BLACK);
  clear_z_buffer();
  draw_grid_points(C_GUNMETAL);

  // Loopeamos los triangulos a renderizar y los dibujamos.
  int num_triangles = num_triangles_to_render;
  for (int i = 0; i < num_triangles; i++) {
    triangle_t triangle = triangles_to_render[i];
    if (should_render_filled_triangles()) {

      draw_filled_triangle(
          triangle.points[0].x, triangle.points[0].y, triangle.points[0].z,
          triangle.points[0].w, triangle.points[1].x, triangle.points[1].y,
          triangle.points[1].z, triangle.points[1].w, triangle.points[2].x,
          triangle.points[2].y, triangle.points[2].z, triangle.points[2].w,
          triangle.color);
    }

    if (should_render_vertex()) {
      draw_rectangle(triangle.points[0].x, triangle.points[0].y, 3, 3, C_RED);
      draw_rectangle(triangle.points[1].x, triangle.points[1].y, 3, 3, C_RED);
      draw_rectangle(triangle.points[2].x, triangle.points[2].y, 3, 3, C_RED);
    }

    if (should_render_textured_triangles()) {
      draw_textured_triangle(
          triangle.points[0].x, triangle.points[0].y, triangle.points[0].z,
          triangle.points[0].w, triangle.textcoords[0].u,
          triangle.textcoords[0].v, // vertex A
          triangle.points[1].x, triangle.points[1].y, triangle.points[1].z,
          triangle.points[1].w, triangle.textcoords[1].u,
          triangle.textcoords[1].v, // vertex B
          triangle.points[2].x, triangle.points[2].y, triangle.points[2].z,
          triangle.points[2].w, triangle.textcoords[2].u,
          triangle.textcoords[2].v, // vertex C
          triangle.texture);
    }

    if (should_render_wireframe()) {
      draw_triangle(triangle, C_RED);
    }
  }

  // Copiamos color buffer al texture buffer y lo enviamos al renderer.
  render_color_buffer();
}

void free_resources(void) {
  free_meshes();
  destroy_window();
}
