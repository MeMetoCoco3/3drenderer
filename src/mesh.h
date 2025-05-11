#ifndef MESH_H
#define MESH_H

#include "array.h"
#include "display.h"
#include "stddef.h"
#include "triangle.h"
#include "vector.h"
#include <stdlib.h>
#define MAX_LINE_LENGTH 1024

enum OBJ_FILE_TOKENS { V, F };

typedef struct {
  vec3_t *vertices;
  face_t *faces;
  vec3_t rotation;
  vec3_t scale;
  vec3_t translation;
  vec3_t sheer;
} mesh_t;

extern mesh_t mesh;
void load_cube_mesh_data(void);
void load_obj_file_data(char *);
void free_resources(void);

#define N_CUBE_VERTICES 8
extern vec3_t cube_vertices[N_CUBE_VERTICES];

#define N_CUBE_FACES (6 * 2) // 6 faces, 2 triangles per face
extern face_t cube_faces[N_CUBE_FACES];

#endif
