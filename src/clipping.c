#include "clipping.h"
#include <math.h>
plane_t frustum_planes[NUM_PLANES];

polygon_t create_polygon_from_triangle(vec3_t v0, vec3_t v1, vec3_t v2,
                                       tex2_t t0, tex2_t t1, tex2_t t2) {
  polygon_t polygon = {
      .vertices = {v0, v1, v2},
      .texcoords = {t0, t1, t2},
      .num_vertices = 3,

  };
  return polygon;
}

void triangles_from_polygon(polygon_t *polygon, triangle_t triangles[],
                            int *num_triangles_after_clipping) {
  for (int i = 0; i < polygon->num_vertices - 2; i++) {
    int index0 = 0;
    int index1 = i + 1;
    int index2 = i + 2;

    triangles[i].points[0] = vec4_from_vec3(polygon->vertices[index0]);
    triangles[i].points[1] = vec4_from_vec3(polygon->vertices[index1]);
    triangles[i].points[2] = vec4_from_vec3(polygon->vertices[index2]);
    triangles[i].textcoords[0] = polygon->texcoords[index0];
    triangles[i].textcoords[1] = polygon->texcoords[index1];
    triangles[i].textcoords[2] = polygon->texcoords[index2];
  }
  *num_triangles_after_clipping = polygon->num_vertices - 2;
}
void clip_polygon(polygon_t *polygon) {
  clip_polygon_against_plane(polygon, LEFT_FRUSTUM_PLANE);
  clip_polygon_against_plane(polygon, RIGHT_FRUSTUM_PLANE);
  clip_polygon_against_plane(polygon, TOP_FRUSTUM_PLANE);
  clip_polygon_against_plane(polygon, BOTTOM_FRUSTUM_PLANE);
  clip_polygon_against_plane(polygon, NEAR_FRUSTUM_PLANE);
  clip_polygon_against_plane(polygon, FAR_FRUSTUM_PLANE);
}

void clip_polygon_against_plane(polygon_t *polygon, enum FRUSTUM_PLANE plane) {
  vec3_t plane_point = frustum_planes[plane].point;
  vec3_t plane_normal = frustum_planes[plane].normal;

  vec3_t inside_vertices[MAX_NUM_POLYGON_VERTICES];
  tex2_t inside_texcoords[MAX_NUM_POLYGON_VERTICES];
  int num_inside_vertices = 0;

  vec3_t *current_vertex = &polygon->vertices[0];
  tex2_t *current_texcoord = &polygon->texcoords[0];

  vec3_t *previous_vertex = &polygon->vertices[polygon->num_vertices - 1];
  tex2_t *previous_texcoord = &polygon->texcoords[polygon->num_vertices - 1];
  // Nos dice si esta dentro o fuera del frustum
  float current_dot = 0;
  float previous_dot =
      vec3_dot(vec3_sub(*previous_vertex, plane_point), plane_normal);

  while (current_vertex != &polygon->vertices[polygon->num_vertices]) {
    current_dot =
        vec3_dot(vec3_sub(*current_vertex, plane_point), plane_normal);

    // Si es negativo; hemos ido de fuera a dentro o de dentro a fuera.
    if (current_dot * previous_dot < 0) {
      // Calculate interpolation factor, t = dotQ1 / (dotQ1-dotQ2)
      float t = previous_dot / (previous_dot - current_dot);
      // Calculate intersection point, I = Q1 + t(Q2-Q1)
      vec3_t intersection_point = {
          .x = float_lerp(previous_vertex->x, current_vertex->x, t),
          .y = float_lerp(previous_vertex->y, current_vertex->y, t),
          .z = float_lerp(previous_vertex->z, current_vertex->z, t),
      };
      // vec3_t intersection_point = vec3_clone(current_vertex);
      // intersection_point = vec3_sub(intersection_point, *previous_vertex);
      // intersection_point = vec3_mul(intersection_point, t);
      // intersection_point = vec3_add(intersection_point, *previous_vertex);

      tex2_t interpolated_texcoord = {
          .u = float_lerp(previous_texcoord->u, current_texcoord->u, t),
          .v = float_lerp(previous_texcoord->v, current_texcoord->v, t),
      };
      inside_vertices[num_inside_vertices] = vec3_clone(&intersection_point);
      inside_texcoords[num_inside_vertices] =
          tex2_clone(&interpolated_texcoord);
      num_inside_vertices++;
    }

    if (current_dot > 0) {
      inside_vertices[num_inside_vertices] = vec3_clone(current_vertex);
      inside_texcoords[num_inside_vertices] = tex2_clone(current_texcoord);
      num_inside_vertices++;
    }

    previous_dot = current_dot;
    previous_texcoord = current_texcoord;
    previous_vertex = current_vertex;

    current_vertex++;
    current_texcoord++;
  }

  //  Override the polygon with the inside_vertices
  for (int i = 0; i < num_inside_vertices; i++) {
    polygon->vertices[i] = vec3_clone(&inside_vertices[i]);
    polygon->texcoords[i] = tex2_clone(&inside_texcoords[i]);
  }
  polygon->num_vertices = num_inside_vertices;
}

float float_lerp(float a, float b, float t) { return a + t * (b - a); }

void init_frustum_planes(float fov_x, float fov_y, float z_near, float z_far) {
  float cos_half_fov_x = cos(fov_x / 2);
  float sin_half_fov_x = sin(fov_x / 2);
  float cos_half_fov_y = cos(fov_y / 2);
  float sin_half_fov_y = sin(fov_y / 2);

  frustum_planes[LEFT_FRUSTUM_PLANE].point = vec3_new(0, 0, 0);
  frustum_planes[LEFT_FRUSTUM_PLANE].normal.x = cos_half_fov_x;
  frustum_planes[LEFT_FRUSTUM_PLANE].normal.y = 0;
  frustum_planes[LEFT_FRUSTUM_PLANE].normal.z = sin_half_fov_x;

  frustum_planes[RIGHT_FRUSTUM_PLANE].point = vec3_new(0, 0, 0);
  frustum_planes[RIGHT_FRUSTUM_PLANE].normal.x = -cos_half_fov_x;
  frustum_planes[RIGHT_FRUSTUM_PLANE].normal.y = 0;
  frustum_planes[RIGHT_FRUSTUM_PLANE].normal.z = sin_half_fov_x;

  frustum_planes[TOP_FRUSTUM_PLANE].point = vec3_new(0, 0, 0);
  frustum_planes[TOP_FRUSTUM_PLANE].normal.x = 0;
  frustum_planes[TOP_FRUSTUM_PLANE].normal.y = -cos_half_fov_y;
  frustum_planes[TOP_FRUSTUM_PLANE].normal.z = sin_half_fov_y;

  frustum_planes[BOTTOM_FRUSTUM_PLANE].point = vec3_new(0, 0, 0);
  frustum_planes[BOTTOM_FRUSTUM_PLANE].normal.x = 0;
  frustum_planes[BOTTOM_FRUSTUM_PLANE].normal.y = cos_half_fov_y;
  frustum_planes[BOTTOM_FRUSTUM_PLANE].normal.z = sin_half_fov_y;

  frustum_planes[NEAR_FRUSTUM_PLANE].point = vec3_new(0, 0, z_near);
  frustum_planes[NEAR_FRUSTUM_PLANE].normal.x = 0;
  frustum_planes[NEAR_FRUSTUM_PLANE].normal.y = 0;
  frustum_planes[NEAR_FRUSTUM_PLANE].normal.z = 1;

  frustum_planes[FAR_FRUSTUM_PLANE].point = vec3_new(0, 0, z_far);
  frustum_planes[FAR_FRUSTUM_PLANE].normal.x = 0;
  frustum_planes[FAR_FRUSTUM_PLANE].normal.y = 0;
  frustum_planes[FAR_FRUSTUM_PLANE].normal.z = -1;
}
