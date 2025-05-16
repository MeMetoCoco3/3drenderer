#ifndef TRIANGLE_H
#define TRIANGLE_H

#include "texture.h"
#include "vector.h"
#include <stdint.h>

// Stores index of vertex in the mesh, in order clockwise or anticlockwise.
typedef struct {
  int a;
  int b;
  int c;

  tex2_t a_uv;
  tex2_t b_uv;
  tex2_t c_uv;
  uint32_t color;
} face_t;

// Stores value of 3 vertex of a triangle, projected on a screen.
// Usaremos un vec4_t ya que necesitamos pasar z y w a draw_texture_triangle
// para corregir la perspectiva de la textura dibujada en los triangulos.
typedef struct {
  vec4_t points[3];
  tex2_t textcoords[3];
  uint32_t color;
  float avg_depth;

} triangle_t;

void draw_texel(int x, int y, vec4_t point_a, vec4_t point_b, vec4_t point_c,
                tex2_t a_uv, tex2_t b_uv, tex2_t c_uv,
                // float u0, float v0, float u1, float v1, float u2, float v2,
                uint32_t *texture);
void draw_filled_triangle(int x0, int y0, int x1, int y1, int x2, int y2,
                          uint32_t color);
void draw_textured_triangle(int x0, int y0, float z0, float w0, float u0,
                            float v0, int x1, int y1, float z1, float w1,
                            float u1, float v1, int x2, int y2, float z2,
                            float w2, float u2, float v2, uint32_t *texture);
void fill_flat_bottom_triangle(int x0, int y0, int x1, int y1, int mx, int my,
                               uint32_t color);
void fill_flat_top_triangle(int x1, int y1, int mx, int my, int x2, int y2,
                            uint32_t color);
vec3_t barycentric_weights(vec2_t a, vec2_t b, vec2_t c, vec2_t p);

#endif
