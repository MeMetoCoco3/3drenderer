#ifndef CLIPPING_H
#define CLIPPING_H
#include "triangle.h"
#include "vector.h"
#define NUM_PLANES 6
#define MAX_NUM_POLYGON_VERTICES 10
#define MAX_NUM_POLYGON_TRIANGLES 10

enum FRUSTUM_PLANE {
  LEFT_FRUSTUM_PLANE,
  RIGHT_FRUSTUM_PLANE,
  TOP_FRUSTUM_PLANE,
  BOTTOM_FRUSTUM_PLANE,
  NEAR_FRUSTUM_PLANE,
  FAR_FRUSTUM_PLANE
};

typedef struct {
  vec3_t vertices[MAX_NUM_POLYGON_VERTICES];
  tex2_t texcoords[MAX_NUM_POLYGON_VERTICES];
  int num_vertices;
} polygon_t;

typedef struct {
  vec3_t point;
  vec3_t normal;
} plane_t;

polygon_t create_polygon_from_triangle(vec3_t v0, vec3_t v1, vec3_t v2,
                                       tex2_t t0, tex2_t t1, tex2_t t2);
void clip_polygon(polygon_t *polygon);
void clip_polygon_against_plane(polygon_t *poligon, enum FRUSTUM_PLANE plane);
float float_lerp(float a, float b, float t);
void init_frustum_planes(float fov_x, float fov_y, float z_near, float z_far);
void triangles_from_polygon(polygon_t *polygon, triangle_t triangles[],
                            int *num_triangles_after_clipping);
#endif
