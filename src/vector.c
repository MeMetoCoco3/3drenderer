#include "vector.h"
#include <math.h>

float fov_factor = 720;
int depth = 5;
vec3_t camera_position = {.x = 0, .y = 0, .z = 0};

vec2_t vec2_new(float x, float y) {
  vec2_t res = {x, y};
  return res;
}

vec2_t vec2_from_vec4(vec4_t a) {
  vec2_t res = {.x = a.x, .y = a.y};
  return res;
}

float vec2_length(vec2_t v) { return sqrt(v.x * v.x + v.y * v.y); }
vec2_t vec2_add(vec2_t a, vec2_t b) {
  vec2_t res = {a.x + b.x, a.y + b.y};
  return res;
}
vec2_t vec2_sub(vec2_t a, vec2_t b) {
  vec2_t res = {a.x - b.x, a.y - b.y};
  return res;
}

vec2_t vec2_mul(vec2_t v, float factor) {
  vec2_t res = {v.x * factor, v.y * factor};
  return res;
}

vec2_t vec2_div(vec2_t v, float factor) {
  vec2_t res = {v.x / factor, v.y / factor};
  return res;
}

float vec2_dot(vec2_t a, vec2_t b) { return (a.x * b.x) + (a.y * b.y); }

void vec2_normalize(vec2_t *v) {
  float length = sqrt(v->x * v->x + v->y * v->y);
  v->x /= length;
  v->y /= length;
}

vec3_t vec3_clone(vec3_t *v) {
  vec3_t res = {v->x, v->y, v->z};
  return res;
}

vec3_t vec3_new(float x, float y, float z) {
  vec3_t res = {x, y, z};
  return res;
}

float vec3_length(vec3_t v) { return sqrt(v.x * v.x + v.y * v.y + v.z * v.z); }

vec3_t vec3_add(vec3_t a, vec3_t b) {
  vec3_t res = {a.x + b.x, a.y + b.y, a.z + b.z};
  return res;
}
vec3_t vec3_sub(vec3_t a, vec3_t b) {
  vec3_t res = {a.x - b.x, a.y - b.y, a.z - b.z};
  return res;
}

vec3_t vec3_mul(vec3_t v, float factor) {
  vec3_t res = {v.x * factor, v.y * factor, v.z * factor};
  return res;
}

vec3_t vec3_div(vec3_t v, float factor) {
  vec3_t res = {v.x / factor, v.y / factor, v.z / factor};
  return res;
}

vec3_t vec3_cross(vec3_t a, vec3_t b) {
  vec3_t res = {
      .x = a.y * b.z - a.z * b.y,
      .y = a.z * b.x - a.x * b.z,
      .z = a.x * b.y - a.y * b.x,
  };
  return res;
}

float vec3_dot(vec3_t a, vec3_t b) {
  return (a.x * b.x) + (a.y * b.y) + (a.z * b.z);
}

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

void vec3_normalize(vec3_t *v) {
  float length = sqrt(v->x * v->x + v->y * v->y + v->z * v->z);
  v->x /= length;
  v->y /= length;
  v->z /= length;
}

vec4_t vec4_from_vec3(vec3_t v) {
  vec4_t v4 = {.x = v.x, .y = v.y, .z = v.z, .w = 1.0};
  return v4;
}
vec3_t vec3_from_vec4(vec4_t v) {
  vec3_t v3 = {.x = v.x, .y = v.y, .z = v.z};
  return v3;
}

int clamp(int value, int min, int max) {
  if (value < min)
    return min;
  if (value > max)
    return max;
  return value;
}

/* vec2_t project(vec3_t point) { */
/*   vec2_t point_2d = {.x = point.x * fov_factor, .y = point.y * fov_factor};
 */
/*   return point_2d; */
/* } */
