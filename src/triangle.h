#ifndef TRIANGLE_H
#define TRIANGLE_H

#include "vector.h"
// Stores index of vertex in the mesh, in order clockwise or anticlockwise.
typedef struct {
  int a;
  int b;
  int c;
} face_t;

// Stores value of 3 vertex of a triangle, projected on a screen.
typedef struct {
  vec2_t points[3];
} triangle_t;
#endif
