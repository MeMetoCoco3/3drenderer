#include "array.h"
#include "colors.h"
#include "display.h"
#include "light.h"
#include "matrix.h"
#include "mesh.h"
#include "sort.h"
#include "triangle.h"
#include "upng.h"
#include "vector.h"
#include <math.h>

bool is_running;

int previous_frame_time = 0;
triangle_t *triangles_to_render = NULL;
rendering_data_t rendering_data;
mat4_t projection_matrix;
light_t light_source;

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
  // Aplicamos default values de como queremos renderizar nuestros modelos.
  rendering_data.rm = RM_COLORED;
  rendering_data.bc = BACKFACE_CULLING_ON;
  rendering_data.l = LIGHTS_OF;
  // Creamos un color_buffer con el tamaño de W*H*sizeof(pixel).
  color_buffer =
      (uint32_t *)malloc(sizeof(uint32_t) * window_width * window_height);
  z_buffer = (float *)malloc(sizeof(float) * window_width * window_height);
  // Creamos una textura donde copiaremos el color buffer.
  // Esta textura sera interpretada por el renderer.
  // Pixel Format = AARRGGBB;
  // Lo cambie ya que las texturas estan codificadas en RGBA format.
  // Text Access straming = Recurriremos a este bloque de memoria
  // constantemente.
  color_buffer_texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA32,
                                           SDL_TEXTUREACCESS_STREAMING,
                                           window_width, window_height);

  // Matrix Projection.
  // FOV en radianes (equivalente a 60 grados).
  float FOV = 3.14159 / 3;
  // Ratio entre altura y ancho de pantalla.
  float aspect = (float)window_height / (float)window_width;
  // Cuan cerca y cuan lejos podemos percibir nuestros objetos.
  float znear = 0.1;
  float zfar = 100.0;
  projection_matrix = mat4_make_perspective(FOV, aspect, znear, zfar);

  light_source = (light_t){.direction = {.x = 1000, .y = -200, .z = -1000}};

  // Estas dos funciones se encargan de:
  // Cargar objetos en nuestra mesh.
  load_obj_file_data("./assets/efa.obj");

  // Cargar datos predefinidos en nuestra mesh.
  // load_cube_mesh_data();

  // Cargamos los datos de la texture en la memoria.
  load_png_texture_data("./assets/efa.png");
}

void get_input(void) {
  // Estructura encargada de lidiar con todos los eventos de nuestro SO.
  // Utilizado para lidiar con keyboard input.
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
      rendering_data.rm = RM_WIREFRAME_VERTEX;
    }
    if (event.key.keysym.sym == SDLK_KP_3) {
      rendering_data.rm = RM_COLORED;
    }
    if (event.key.keysym.sym == SDLK_KP_4) {
      rendering_data.rm = RM_COLORED_LINES;
    }
    if (event.key.keysym.sym == SDLK_KP_5) {
      rendering_data.rm = RM_TEXTURED;
    }
    if (event.key.keysym.sym == SDLK_KP_6) {
      rendering_data.rm = RM_TEXTURED_WIRE;
    }
    if (event.key.keysym.sym == SDLK_c) {
      rendering_data.bc = BACKFACE_CULLING_ON;
    }
    if (event.key.keysym.sym == SDLK_d) {
      rendering_data.l = LIGHTS_OF;
      rendering_data.bc = BACKFACE_CULLING_OF;
    }
    if (event.key.keysym.sym == SDLK_k) {
      printf("LIGHTS ON");
      rendering_data.bc = BACKFACE_CULLING_ON;
      rendering_data.l = LIGHTS_ON;
    }
    break;
  }
}

void update(void) {

  // Deltatime.
  // Definimos cuantos ticks tenemos que esperar para que los frames no
  // se dibujen mas rapido de lo que deberian.
  int time_to_wait = FRAME_TARGET_TIME - (SDL_GetTicks() - previous_frame_time);
  if (time_to_wait > 0 && time_to_wait <= FRAME_TARGET_TIME) {
    SDL_Delay(time_to_wait);
  }
  previous_frame_time = SDL_GetTicks();

  // Definimos DynamicArray para agrupar nuestros triangulos.
  triangles_to_render = NULL;

  // Movemos, rotamos, escalamos, nuestra mesh.
  // Crearemos matrices a partir de estos valores para multiplicar cada matriz
  // con la world_matrix.
  mesh.rotation.x += 0.015f;
  // mesh.rotation.y += 0.02f;
  // mesh.rotation.z += 0.02f;

  // mesh.scale.x += 0.001f;
  // mesh.scale.y += 0.001f;

  // mesh.translation.x += 0.01f;
  // mesh.translation.y += 0.01f;
  mesh.translation.z = depth;
  // mesh.sheer.x += 0.01f;

  // El orden importa, siempre aplicaremos antes:
  // 1. Scale.
  // 2. Rotation z,y,x.
  // 3. Translation.
  // 0. Sheer debe ser aplicado antes o despues que scale.
  mat4_t scale_matrix =
      mat4_make_scale(mesh.scale.x, mesh.scale.y, mesh.scale.z);
  mat4_t translation_matrix = mat4_make_translation(
      mesh.translation.x, mesh.translation.y, mesh.translation.z);
  mat4_t rotation_matrix_x = mat4_make_rotation_x(mesh.rotation.x);
  mat4_t rotation_matrix_y = mat4_make_rotation_y(mesh.rotation.y);
  mat4_t rotation_matrix_z = mat4_make_rotation_z(mesh.rotation.z);

  // mat4_t sheer_matrix_x = mat4_make_shear_x(mesh.sheer.y, mesh.sheer.z);
  // mat4_t sheer_matrix_y = mat4_make_shear_y(mesh.sheer.x, mesh.sheer.z);
  // mat4_t sheer_matrix_z = mat4_make_shear_z(mesh.sheer.x, mesh.sheer.y);

  // Loopeamos entre todas nuestras caras.
  // Cada cara tiene 3 int, y 1 color.
  // Los int representando el indice del vertice que forma la cara.
  int num_faces = array_length(mesh.faces);
  for (int i = 0; i < num_faces; i++) {
    // Extraemos los correspondientes vertices a cada indice.
    face_t face = mesh.faces[i];
    vec3_t face_vertices[3];
    uint32_t final_color;
    face_vertices[0] = mesh.vertices[face.a];
    face_vertices[1] = mesh.vertices[face.b];
    face_vertices[2] = mesh.vertices[face.c];

    // Transformamos los vertices.
    // Como algunas operaciones como translation, o la conservacion del valor Z
    // de cada punto requieren de una matriz n+1, trabajaremos con vec4_t.
    vec4_t transformed_vertices[3];
    // Transformation
    for (int j = 0; j < 3; j++) {
      // Creamos una matriz mundo, le aplicamos todas las matrices declaradas
      // anteriormente. Y despues aplicaremos el resultado a los vertices.
      vec4_t transformed_vertex = vec4_from_vec3(face_vertices[j]);

      mat4_t world_matrix = mat4_identity();

      // world_matrix = mat4_mul_mat4(sheer_matrix_x, world_matrix);
      // world_matrix = mat4_mul_mat4(sheer_matrix_y, world_matrix);
      // world_matrix = mat4_mul_mat4(sheer_matrix_z, world_matrix);

      world_matrix = mat4_mul_mat4(scale_matrix, world_matrix);
      world_matrix = mat4_mul_mat4(rotation_matrix_z, world_matrix);
      world_matrix = mat4_mul_mat4(rotation_matrix_y, world_matrix);
      world_matrix = mat4_mul_mat4(rotation_matrix_x, world_matrix);
      world_matrix = mat4_mul_mat4(translation_matrix, world_matrix);

      transformed_vertices[j] = mat4_mul_vec4(world_matrix, transformed_vertex);
    }

    // Backface culling
    // Algoritmo encargado de decidir si las caras deben ser dibujadas o no,
    // basandonos en el angulo que el normal, de una cara forma con nuestra
    // camara.
    if (rendering_data.bc == BACKFACE_CULLING_ON) {
      vec3_t vector_a = vec3_from_vec4(transformed_vertices[0]);
      vec3_t vector_b = vec3_from_vec4(transformed_vertices[1]);
      vec3_t vector_c = vec3_from_vec4(transformed_vertices[2]);

      // Sacamos la direccion de dos vectores con mismo origen de la cara.
      vec3_t vector_ab = vec3_sub(vector_b, vector_a);
      vec3_t vector_ac = vec3_sub(vector_c, vector_a);
      vec3_normalize(&vector_ab);
      vec3_normalize(&vector_ac);
      // El CrossProduct the dos vectores nos da un tercer vector perpendicular
      // a estos, o la normal de la cara formada por dichos vectores.
      vec3_t normal = vec3_cross(vector_ab, vector_ac);
      vec3_normalize(&normal);

      // Calculamos vector entre la camara y el punto que estamos chekeando.
      vec3_t camera_ray = vec3_sub(camera_position, vector_a);

      // El dot product nos da el angulo entre dos vectores.
      float angle = vec3_dot(normal, camera_ray);

      // Si el angulo es negativo o 0, no dibujaremos esa cara.
      if (angle < 0.0) {
        continue;
      }
    }

    // Calculate new colors with light
    if (rendering_data.l == LIGHTS_ON) {
      vec3_t vector_a = vec3_from_vec4(transformed_vertices[0]);
      vec3_t vector_b = vec3_from_vec4(transformed_vertices[1]);
      vec3_t vector_c = vec3_from_vec4(transformed_vertices[2]);

      vec3_t vector_ab = vec3_sub(vector_b, vector_a);
      vec3_t vector_ac = vec3_sub(vector_c, vector_a);
      vec3_normalize(&vector_ab);
      vec3_normalize(&vector_ac);

      vec3_t normal = vec3_cross(vector_ab, vector_ac);
      vec3_t light_source_direction = light_source.direction;
      vec3_normalize(&light_source_direction);
      vec3_normalize(&normal);

      // vec3_t light_ray = vec3_sub(light_source.direction, vector_a);
      float factor = vec3_dot(normal, light_source_direction);
      if (factor < 0.2f)
        factor = 0.2f;
      if (factor > 1.0f)
        factor = 1.0f;

      final_color = light_apply_intensity(mesh.faces[i].color, factor);
    } else {
      final_color = mesh.faces[i].color;
    }
    // Projection
    vec4_t projected_points[3];
    for (int j = 0; j < 3; j++) {
      // Multiplicamos los vectores por la matriz de projeccion.
      // En la funcion aplicamos perspective divide.
      // El resultado esta en un rango de -1 a 1 en x, y, z, llamado
      // NDC(Normalized Device Coordinates).
      projected_points[j] =
          mat4_mul_vec4_project(projection_matrix, transformed_vertices[j]);

      projected_points[j].y *= -1;
      // Escalar a Screen Coordinates.
      projected_points[j].x *= (window_width / 2.0);
      projected_points[j].y *= (window_height / 2.0);

      // Trasladar a Screen Coordinates.
      projected_points[j].x += (window_width / 2.0);
      projected_points[j].y += (window_height / 2.0);
    }

    triangle_t projected_triangle = {
        .points =
            {
                {projected_points[0].x, projected_points[0].y,
                 projected_points[0].z, projected_points[0].w},
                {projected_points[1].x, projected_points[1].y,
                 projected_points[1].z, projected_points[1].w},
                {projected_points[2].x, projected_points[2].y,
                 projected_points[2].z, projected_points[2].w},
            },

        .textcoords = {{face.a_uv.u, face.a_uv.v},
                       {face.b_uv.u, face.b_uv.v},
                       {face.c_uv.u, face.c_uv.v}},
        .color = final_color,
    };

    array_push(triangles_to_render, projected_triangle);
  }
  // quick_sort(triangles_to_render, 0, array_length(triangles_to_render) - 1);
}

void render(void) {
  SDL_RenderClear(renderer);

  // Dibujamos grid.
  draw_grid_points(C_GUNMETAL);

  // Loopeamos los triangulos a renderizar y los dibujamos.
  int num_triangles = array_length(triangles_to_render);
  for (int i = 0; i < num_triangles; i++) {
    triangle_t triangle = triangles_to_render[i];
    if (rendering_data.rm == RM_COLORED ||
        rendering_data.rm == RM_COLORED_LINES) {

      draw_filled_triangle(
          triangle.points[0].x, triangle.points[0].y, triangle.points[0].z,
          triangle.points[0].w, triangle.points[1].x, triangle.points[1].y,
          triangle.points[1].z, triangle.points[1].w, triangle.points[2].x,
          triangle.points[2].y, triangle.points[2].z, triangle.points[2].w,
          triangle.color);
    }

    if (rendering_data.rm == RM_WIREFRAME_VERTEX) {
      draw_rectangle(triangle.points[0].x, triangle.points[0].y, 3, 3, C_RED);
      draw_rectangle(triangle.points[1].x, triangle.points[1].y, 3, 3, C_RED);
      draw_rectangle(triangle.points[2].x, triangle.points[2].y, 3, 3, C_RED);
    }

    if (rendering_data.rm == RM_TEXTURED ||
        rendering_data.rm == RM_TEXTURED_WIRE) {
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
          mesh_texture);
    }

    if (rendering_data.rm == RM_COLORED_LINES ||
        rendering_data.rm == RM_WIREFRAME ||
        rendering_data.rm == RM_WIREFRAME_VERTEX ||
        rendering_data.rm == RM_TEXTURED_WIRE) {
      draw_triangle(triangle, C_RED);
    }
  }
  // Liberamos los triangulos.
  array_free(triangles_to_render);
  // Copiamos color buffer al texture buffer y lo enviamos al renderer.
  render_color_buffer();
  // Limpiamos el color buffer.
  clear_color_buffer(C_BLACK);
  clear_z_buffer();
  // Actualizamos la pantalla.
  SDL_RenderPresent(renderer);
}
