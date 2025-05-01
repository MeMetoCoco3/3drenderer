#ifndef VECTOR_H
#define VECTOR_H

extern float fov_factor;
typedef struct {
  float x;
  float y;
} vec2_t;

typedef struct {
  float x;
  float y;
  float z;
} vec3_t;

vec2_t project(vec3_t point);

#endif
