#include "vector.h"

float fov_factor = 120;

vec2_t project(vec3_t point) {
  vec2_t point_2d = {.x = (point.x + 1.0f) * fov_factor,
                     .y = (point.y + 1.0f) * fov_factor};
  return point_2d;
}
