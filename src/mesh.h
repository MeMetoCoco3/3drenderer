#ifndef MESH_H
#define MESH_H

#include "array.h"
#include "stddef.h"
#include "triangle.h"
#include "vector.h"
typedef struct {
  vec3_t *vertices;
  face_t *faces;
  vec3_t rotation;
} mesh_t;

void load_cube_mesh_data(void);
extern mesh_t mesh;

#define N_CUBE_VERTICES 8
extern vec3_t cube_vertices[N_CUBE_VERTICES];

#define N_CUBE_FACES (6 * 2) // 6 faces, 2 triangles per face
extern face_t cube_faces[N_CUBE_FACES];

#endif
