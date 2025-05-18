#include "triangle.h"
#include "display.h"
#include "sort.h"
#include "vector.h"

void draw_texel(int x, int y, vec4_t point_a, vec4_t point_b, vec4_t point_c,
                tex2_t a_uv, tex2_t b_uv, tex2_t c_uv, u_int32_t *texture) {

  vec2_t a = vec2_from_vec4(point_a);
  vec2_t b = vec2_from_vec4(point_b);
  vec2_t c = vec2_from_vec4(point_c);
  vec2_t center = {x, y};

  vec3_t weights = barycentric_weights(a, b, c, center);

  float alpha = weights.x;
  float beta = weights.y;
  float gamma = weights.z;

  float interpolated_u;
  float interpolated_v;
  float interpolated_reciprocal_w;

  // Interpolamos u/w y v/w usando el barycentric weight
  // Lo mismo con la inversa de W para calcular reciprocal_w
  // Valores entre 0 y 1.
  interpolated_u = (a_uv.u / point_a.w) * alpha + (b_uv.u / point_b.w) * beta +
                   (c_uv.u / point_c.w) * gamma;
  interpolated_v = (a_uv.v / point_a.w) * alpha + (b_uv.v / point_b.w) * beta +
                   (c_uv.v / point_c.w) * gamma;

  interpolated_reciprocal_w = (1 / point_a.w) * alpha + (1 / point_b.w) * beta +
                              (1 / point_c.w) * gamma;

  interpolated_u /= interpolated_reciprocal_w;
  interpolated_v /= interpolated_reciprocal_w;

  // interpolated_reciprocal_w es un valor mayor conforme se acerca a znear, por
  interpolated_reciprocal_w = 1.0 - interpolated_reciprocal_w;

  if (interpolated_reciprocal_w < z_buffer[x + (window_width * y)]) {
    // Escalamos U y V al tamaño de la textura.
    int text_x = abs((int)(interpolated_u * texture_width)) % texture_width;
    int text_y = abs((int)(interpolated_v * texture_height)) % texture_height;

    draw_pixel(x, y, texture[(texture_width * text_y) + text_x]);
    z_buffer[x + (window_width * y)] = interpolated_reciprocal_w;
  }
}

void draw_triangle_pixel(int x, int y, vec4_t point_a, vec4_t point_b,
                         vec4_t point_c, u_int32_t color) {
  vec2_t a = vec2_from_vec4(point_a);
  vec2_t b = vec2_from_vec4(point_b);
  vec2_t c = vec2_from_vec4(point_c);
  vec2_t center = {x, y};

  vec3_t weights = barycentric_weights(a, b, c, center);

  float alpha = weights.x;
  float beta = weights.y;
  float gamma = weights.z;

  float interpolated_reciprocal_w = (1 / point_a.w) * alpha +
                                    (1 / point_b.w) * beta +
                                    (1 / point_c.w) * gamma;

  interpolated_reciprocal_w = 1.0 - interpolated_reciprocal_w;

  if (interpolated_reciprocal_w < z_buffer[x + y * window_width]) {
    draw_pixel(x, y, color);
    z_buffer[x + y * window_width] = interpolated_reciprocal_w;
  }
}

void draw_filled_triangle(int x0, int y0, float z0, float w0, int x1, int y1,
                          float z1, float w1, int x2, int y2, float z2,
                          float w2, uint32_t color) {
  if (y0 > y1) {
    swap_int(&y0, &y1);
    swap_int(&x0, &x1);
    swap_f(&z0, &z1);
    swap_f(&w0, &w1);
  }
  if (y1 > y2) {
    swap_int(&y1, &y2);
    swap_int(&x1, &x2);
    swap_f(&z1, &z2);
    swap_f(&w1, &w2);
  }
  if (y0 > y1) {
    swap_int(&y0, &y1);
    swap_int(&x0, &x1);
    swap_f(&z0, &z1);
    swap_f(&w0, &w1);
  }

  vec4_t point_a = {x0, y0, z0, w0};
  vec4_t point_b = {x1, y1, z1, w1};
  vec4_t point_c = {x2, y2, z2, w2};
  // Dibujamos el flat bottom triangle.
  float inverse_slope_1 = 0;
  float inverse_slope_2 = 0;

  // TODO: Esta linea es reiterativa
  if (y1 - y0 != 0) {
    inverse_slope_1 = (float)(x1 - x0) / abs(y1 - y0);
  }
  if (y2 - y0 != 0) {
    inverse_slope_2 = (float)(x2 - x0) / abs(y2 - y0);
  }

  if (y1 - y0 != 0) {
    for (int y = y0; y <= y1; y++) {
      int x_start = x1 + (y - y1) * inverse_slope_1;
      int x_end = x0 + (y - y0) * inverse_slope_2;

      // forzaremos que x_start siempre sea el valor menor ya que dibujamos de
      // izquierda a derecha.
      if (x_end < x_start) {
        swap_int(&x_start, &x_end);
      }
      for (int x = x_start; x < x_end; x++) {
        draw_triangle_pixel(x, y, point_a, point_b, point_c, color);
      }
    }
  }

  // Dibujamos el flat top triangle.
  if (y2 - y1 != 0) {
    inverse_slope_1 = (float)(x2 - x1) / abs(y2 - y1);
  }
  if (y2 - y0 != 0) {
    inverse_slope_2 = (float)(x2 - x0) / abs(y2 - y0);
  }

  if (y2 - y1 != 0) {
    for (int y = y1; y <= y2; y++) {
      int x_start = x1 + (y - y1) * inverse_slope_1;
      int x_end = x0 + (y - y0) * inverse_slope_2;

      if (x_start > x_end) {
        swap_int(&x_start, &x_end);
      }
      for (int x = x_start; x < x_end; x++) {
        draw_triangle_pixel(x, y, point_a, point_b, point_c, color);
      }
    }
  }
}

// void draw_filled_triangle(int x0, int y0, int x1, int y1, int x2, int y2,
//                           uint32_t color) {
//   // Necesitamos los vertices ordenados en el eje vertical.
//   // y0 < y1 < y2
//   if (y0 > y1) {
//     swap_int(&y0, &y1);
//     swap_int(&x0, &x1);
//   }
//   if (y1 > y2) {
//     swap_int(&y1, &y2);
//     swap_int(&x1, &x2);
//   }
//   if (y0 > y1) {
//     swap_int(&y0, &y1);
//     swap_int(&x0, &x1);
//   }
//   if (y1 == y2) {
//     fill_flat_bottom_triangle(x0, y0, x1, y1, x2, y2, color);
//   } else if (y0 == y1) {
//     fill_flat_top_triangle(x0, y0, x1, y1, x2, y2, color);
//   } else {
//
//     int my = y1;
//     // int mx = ((float)((x2 - x0) * (y1 - y0)) / (float)(y2 - y0)) + x0;
//     float alpha = (float)(y1 - y0) / (float)(y2 - y0);
//     float mx = x0 + (x2 - x0) * alpha;
//     fill_flat_bottom_triangle(x0, y0, x1, y1, mx, my, color);
//     fill_flat_top_triangle(x1, y1, mx, my, x2, y2, color);
//   }
// }
//
void draw_textured_triangle(int x0, int y0, float z0, float w0, float u0,
                            float v0, int x1, int y1, float z1, float w1,
                            float u1, float v1, int x2, int y2, float z2,
                            float w2, float u2, float v2, uint32_t *texture) {
  // Necesitamos los vertices ordenados en el eje vertical.
  // y0 < y1 < y2
  if (y0 > y1) {
    swap_int(&y0, &y1);
    swap_int(&x0, &x1);
    swap_f(&z0, &z1);
    swap_f(&w0, &w1);
    swap_f(&u0, &u1);
    swap_f(&v0, &v1);
  }
  if (y1 > y2) {
    swap_int(&y1, &y2);
    swap_int(&x1, &x2);
    swap_f(&z1, &z2);
    swap_f(&w1, &w2);
    swap_f(&u1, &u2);
    swap_f(&v1, &v2);
  }
  if (y0 > y1) {
    swap_int(&y0, &y1);
    swap_int(&x0, &x1);
    swap_f(&z0, &z1);
    swap_f(&w0, &w1);
    swap_f(&u0, &u1);
    swap_f(&v0, &v1);
  }
  v0 = 1.0 - v0;
  v1 = 1.0 - v1;
  v2 = 1.0 - v2;

  vec4_t point_a = {x0, y0, z0, w0};
  vec4_t point_b = {x1, y1, z1, w1};
  vec4_t point_c = {x2, y2, z2, w2};
  tex2_t a_uv = {u0, v0};
  tex2_t b_uv = {u1, v1};
  tex2_t c_uv = {u2, v2};

  // Dibujamos el flat bottom triangle.
  float inverse_slope_1 = 0;
  float inverse_slope_2 = 0;
  if (y1 - y0 != 0) {
    inverse_slope_1 = (float)(x1 - x0) / abs(y1 - y0);
  }
  if (y2 - y0 != 0) {
    inverse_slope_2 = (float)(x2 - x0) / abs(y2 - y0);
  }

  if (y1 - y0 != 0) {
    for (int y = y0; y <= y1; y++) {
      int x_start = x1 + (y - y1) * inverse_slope_1;
      int x_end = x0 + (y - y0) * inverse_slope_2;

      // forzaremos que x_start siempre sea el valor menor ya que dibujamos de
      // izquierda a derecha.
      if (x_end < x_start) {
        swap_int(&x_start, &x_end);
      }
      for (int x = x_start; x < x_end; x++) {
        // draw_pixel(x, y, (x % 2 == 0 && y % 2 == 0) ? 0xFFFF00FF :
        // 0xFF000000);

        draw_texel(x, y, point_a, point_b, point_c, a_uv, b_uv, c_uv, texture);
      }
    }
  }

  // Dibujamos el flat top triangle.
  if (y2 - y1 != 0) {
    inverse_slope_1 = (float)(x2 - x1) / abs(y2 - y1);
  }
  if (y2 - y0 != 0) {
    inverse_slope_2 = (float)(x2 - x0) / abs(y2 - y0);
  }

  if (y2 - y1 != 0) {
    for (int y = y1; y <= y2; y++) {
      int x_start = x1 + (y - y1) * inverse_slope_1;
      int x_end = x0 + (y - y0) * inverse_slope_2;

      // Forzaremos que x_start siempre sea el valor menor ya que dibujamos de
      // izquierda a derecha.
      if (x_start > x_end) {
        swap_int(&x_start, &x_end);
      }
      for (int x = x_start; x < x_end; x++) {
        draw_texel(x, y, point_a, point_b, point_c, a_uv, b_uv, c_uv, texture);
        // draw_pixel(x, y, (x % 2 == 0 && y % 2 == 0) ? 0xFFFF00FF :
        // 0xFF000000);
      }
    }
  }
}

void fill_flat_bottom_triangle(int x0, int y0, int x1, int y1, int mx, int my,
                               uint32_t color) {

  // Inverse Slope:
  // Teniendo en cuenta que y tiene un incremento de 1 por 1,
  // cuanto incrementamos en x?
  float slope_inverse_1 = (float)(x1 - x0) / (y1 - y0);
  float slope_inverse_2 = (float)(mx - x0) / (my - y0);
  float start_x = x0;
  float end_x = x0;

  for (int i = y0; i <= my; i++) {
    draw_line(start_x, i, end_x, i, color);

    start_x += slope_inverse_1;
    end_x += slope_inverse_2;
  }
}

void fill_flat_top_triangle(int x1, int y1, int mx, int my, int x2, int y2,
                            uint32_t color) {

  // Inverse Slope:
  // Teniendo en cuenta que y tiene un incremento de 1 por 1,
  // cuanto incrementamos en x?
  float slope_inverse_1 = (float)(x2 - x1) / (y2 - y1);
  float slope_inverse_2 = (float)(x2 - mx) / (y2 - my);

  float start_x = x2;
  float end_x = x2;
  for (int y = y2; y >= my; y--) {
    draw_line(start_x, y, end_x, y, color);

    start_x -= slope_inverse_1;
    end_x -= slope_inverse_2;
  }
}

vec3_t barycentric_weights(vec2_t a, vec2_t b, vec2_t c, vec2_t p) {

  // Cogemos los vectores de la siguiente manera (para recibir el area
  // positiva), ya que nuestro renderer utiliza un sistema de coordenadas zurdo:
  // 4 o -------------- o 3
  //   |	        |
  //   |                |
  //   |                |
  //   |                |
  // 1 o -------------- o 2
  //
  //
  // Calculate alpha
  vec2_t ac = vec2_sub(c, a);
  vec2_t ab = vec2_sub(b, a);
  vec2_t ap = vec2_sub(p, a);
  vec2_t pc = vec2_sub(c, p);
  vec2_t pb = vec2_sub(b, p);

  float area_parallelogram_abc = (ac.x * ab.y) - (ac.y * ab.x);

  float alpha = (pc.x * pb.y - pc.y * pb.x) / area_parallelogram_abc;
  float beta = (ac.x * ap.y - ac.y * ap.x) / area_parallelogram_abc;

  // Calculate gamma
  float gamma = 1.0 - alpha - beta;

  vec3_t result = {alpha, beta, gamma};
  return result;
}
