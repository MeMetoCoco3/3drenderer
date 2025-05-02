#include "vector.h"
#include <math.h>

float fov_factor = 640;
vec3_t camera_position = {.x = 0, .y = 0, .z = -5};
vec3_t cube_rotation = {.x = 0, .y = 0, .z = 0};

/* vec2_t project(vec3_t point) { */
/*   vec2_t point_2d = {.x = point.x * fov_factor, .y = point.y * fov_factor};
 */
/*   return point_2d; */
/* } */

vec3_t vec3_rotate_x(vec3_t v, float angle) {
  vec3_t new_vector = {.x = v.x,
                       .y = v.y * cos(angle) - v.z * sin(angle),
                       .z = v.y * sin(angle) + v.z * cos(angle)};
  return new_vector;
}

vec3_t vec3_rotate_y(vec3_t v, float angle) {
  vec3_t new_vector = {.x = v.x * cos(angle) - v.z * sin(angle),
                       .y = v.y,
                       .z = v.x * sin(angle) + v.z * cos(angle)};
  return new_vector;
}
vec3_t vec3_rotate_z(vec3_t v, float angle) {
  vec3_t new_vector = {.x = v.x * cos(angle) - v.y * sin(angle),
                       .y = v.x * sin(angle) + v.y * cos(angle),
                       .z = v.z};
  return new_vector;
}

vec2_t project(vec3_t point) {
  vec2_t point_2d = {.x = (point.x * fov_factor) / point.z,
                     .y = (point.y * fov_factor) / point.z};
  return point_2d;
}
