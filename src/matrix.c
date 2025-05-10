#include "matrix.h"
mat4_t mat4_identity() {
  mat4_t m = {{
      {1, 0, 0, 0},
      {0, 1, 0, 0},
      {0, 0, 1, 0},
      {0, 0, 0, 1},
  }};
  return m;
}

mat4_t mat4_make_scale(float sx, float sy, float sz) {
  mat4_t m = mat4_identity();
  m.values[0][0] = sx;
  m.values[1][1] = sy;
  m.values[2][2] = sz;
  return m;
}

mat4_t mat4_make_translation(float tx, float ty, float tz) {
  mat4_t m = mat4_identity();
  m.values[0][3] = tx;
  m.values[1][3] = ty;
  m.values[2][3] = tz;
  return m;
}

mat4_t mat4_make_rotation_x(float angle) {
  // In 2D we rotate 2 points and the third one is locked, same here.
  mat4_t m = mat4_identity();

  float c = cos(angle);
  float s = sin(angle);
  m.values[1][1] = c;
  m.values[1][2] = -s;

  m.values[2][1] = s;
  m.values[2][2] = c;

  return m;
}
mat4_t mat4_make_rotation_y(float angle) {
  mat4_t m = mat4_identity();

  float c = cos(angle);
  float s = sin(angle);
  m.values[0][0] = c;
  m.values[0][2] = s;

  m.values[2][0] = -s;
  m.values[2][2] = c;

  return m;
}
mat4_t mat4_make_rotation_z(float angle) {
  mat4_t m = mat4_identity();

  float c = cos(angle);
  float s = sin(angle);
  m.values[0][0] = c;
  m.values[0][1] = -s;

  m.values[1][0] = s;
  m.values[1][1] = c;

  return m;
}

vec4_t mat4_mul_vec4(mat4_t m, vec4_t v) {
  vec4_t result;

  result.x = m.values[0][0] * v.x + m.values[0][1] * v.y +
             m.values[0][2] * v.z + m.values[0][3] * v.w;
  result.y = m.values[1][0] * v.x + m.values[1][1] * v.y +
             m.values[1][2] * v.z + m.values[1][3] * v.w;
  result.z = m.values[2][0] * v.x + m.values[2][1] * v.y +
             m.values[2][2] * v.z + m.values[2][3] * v.w;
  result.w = m.values[3][0] * v.x + m.values[3][1] * v.y +
             m.values[3][2] * v.z + m.values[3][3] * v.w;

  return result;
}
