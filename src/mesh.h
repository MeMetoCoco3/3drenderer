#ifndef MESH_H
#define MESH_H

#include "triangle.h"
#include "upng.h"
#include "vector.h"
#define MAX_LINE_LENGTH 1024

enum OBJ_FILE_TOKENS { V, F, VT, VN };

typedef struct {
  vec3_t *vertices;
  face_t *faces;
  upng_t *texture;
  vec3_t rotation;
  vec3_t scale;
  vec3_t translation;
  vec3_t sheer;
} mesh_t;

void load_mesh(char *obj_filename, char *png_filename, vec3_t scale,
               vec3_t translation, vec3_t rotation);
void load_mesh_obj_data(mesh_t *mesh, char *file_name);
void load_mesh_png_data(mesh_t *mesh, char *filename);
void load_obj_file_data(char *file_name);
int get_num_meshes(void);
mesh_t *get_mesh(int);
void free_meshes(void);
int is_line_empty(const char *line);

#endif
