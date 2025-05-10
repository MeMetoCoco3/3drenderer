#ifndef TRIANGLE_H
#define TRIANGLE_H

#include "vector.h"
#include <stdint.h>
// Stores index of vertex in the mesh, in order clockwise or anticlockwise.
typedef struct {
  int a;
  int b;
  int c;

  uint32_t color;
} face_t;

// Stores value of 3 vertex of a triangle, projected on a screen.
typedef struct {
  vec2_t points[3];
  uint32_t color;
  float avg_depth;
} triangle_t;

void draw_filled_triangle(int x0, int y0, int x1, int y1, int x2, int y2,
                          uint32_t color);

void fill_flat_bottom_triangle(int x0, int y0, int x1, int y1, int mx, int my,
                               uint32_t color);
void fill_flat_top_triangle(int x1, int y1, int mx, int my, int x2, int y2,
                            uint32_t color);
#endif
