#include "mesh.h"
#include "array.h"
#include "colors.h"
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_NUM_MESHES 10
static mesh_t meshes[MAX_NUM_MESHES];
static int mesh_count = 0;

void load_mesh(char *obj_filename, char *png_filename, vec3_t scale,
               vec3_t translation, vec3_t rotation) {
  load_mesh_obj_data(&meshes[mesh_count], obj_filename);
  load_mesh_png_data(&meshes[mesh_count], png_filename);

  meshes[mesh_count].scale = scale;
  meshes[mesh_count].translation = translation;
  meshes[mesh_count].rotation = rotation;

  mesh_count++;
}

// v -1.000000 -1.000000 -1.000000
// f 1/1/1 2/2/1 3/3/1
void load_mesh_obj_data(mesh_t *mesh, char *file_name) {
  FILE *fptr;
  fptr = fopen(file_name, "r");

  if (fptr == NULL) {
    printf("Not able to open the file.");
    exit(1);
  }

  tex2_t *texcoords = NULL;
  char line[MAX_LINE_LENGTH];

  while (fgets(line, MAX_LINE_LENGTH, fptr)) {
    if (is_line_empty(line)) {
      continue;
    }

    char *token = strtok(line, " \t\n");
    if (token == NULL || strcmp(token, "#") == 0) {
      continue;
    }

    enum OBJ_FILE_TOKENS kind;
    if (strcmp(token, "f") == 0) {
      kind = F;
    } else if (strcmp(token, "v") == 0) {
      kind = V;
    } else if (strcmp(token, "vt") == 0) {
      kind = VT;
    } else {
      continue;
    }

    switch (kind) {
    case V: {
      float v_values[3];
      for (int i = 0; i < 3; i++) {
        token = strtok(NULL, " \t\n");
        if (token == NULL) {
          printf("Vertex has less than 3 coordinates");
          exit(1);
        }
        v_values[i] = atof(token);
      }

      vec3_t point = {
          .x = v_values[0],
          .y = v_values[1],
          .z = v_values[2],
      };
      array_push(mesh->vertices, point);
      break;
    }
    case F: {
      int f_values[3];
      int t_values[3];
      int n_values[3];

      for (int i = 0; i < 3; i++) {
        token = strtok(NULL, " \t\n");
        if (token == NULL) {
          printf("Face has less than 3 vertex.");
          exit(1);
        }
        sscanf(token, "%d/%d/%d", &f_values[i], &t_values[i], &n_values[i]);
      }

      face_t face = {
          .a = f_values[0] - 1,
          .b = f_values[1] - 1,
          .c = f_values[2] - 1,
          .a_uv = texcoords[t_values[0] - 1],
          .b_uv = texcoords[t_values[1] - 1],
          .c_uv = texcoords[t_values[2] - 1],
          .color = C_BLUE,
      };
      array_push(mesh->faces, face);
      break;
    }
    case VT: {
      float uv_vertices[2];
      for (int i = 0; i < 2; i++) {
        token = strtok(NULL, " \t\n");
        if (token == NULL) {
          printf("Not enough vertex coord.");
          exit(1);
        }
        uv_vertices[i] = atof(token);
      }
      tex2_t textcoord = {
          .u = uv_vertices[0],
          .v = uv_vertices[1],
      };
      array_push(texcoords, textcoord);
      break;
    }
    default:
      printf("Not correctly formated.\n");
      exit(1);
    }
  }
  fclose(fptr);
  array_free(texcoords);
  return;
}
void load_mesh_png_data(mesh_t *mesh, char *filename) {
  upng_t *png_image = upng_new_from_file(filename);
  if (png_image != NULL) {
    upng_decode(png_image);
    if (upng_get_error(png_image) == UPNG_EOK) {
      mesh->texture = png_image;
    } else {
      printf("Failed to decode PNG from file: %s\n", filename);
    }
  } else {
    printf("Failed to load PNG from file: %s\n", filename);
  }
}

int get_num_meshes(void) { return mesh_count; }
mesh_t *get_mesh(int mesh_index) { return &meshes[mesh_index]; }
void free_meshes(void) {
  for (int i = 0; i < mesh_count; i++) {
    upng_free(meshes[i].texture);
    array_free(meshes[i].faces);
    array_free(meshes[i].vertices);
  }
}

int is_line_empty(const char *line) {
  while (*line) {
    if (!isspace((unsigned char)*line)) {
      return 0;
    }
    line++;
  }
  return 1;
}
