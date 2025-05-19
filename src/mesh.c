#include "mesh.h"
#include "colors.h"
#include "triangle.h"
#include "vector.h"
#include <ctype.h>

#include "colors.h"

mesh_t mesh = {
    .vertices = NULL,
    .faces = NULL,
    .rotation = {0, 0, 0},
    .scale = {1.0, 1.0, 1.0},
    .translation = {0, 0, 0},
    .sheer = {0, 0, 0},
};

void load_cube_mesh_data(void) {
  for (int i = 0; i < N_CUBE_VERTICES; i++) {
    array_push(mesh.vertices, cube_vertices[i]);
  }
  for (int i = 0; i < N_CUBE_FACES; i++) {
    array_push(mesh.faces, cube_faces[i]);
  }
}

// v -1.000000 -1.000000 -1.000000
// f 1/1/1 2/2/1 3/3/1

int is_line_empty(const char *line) {
  while (*line) {
    if (!isspace((unsigned char)*line)) {
      return 0;
    }
    line++;
  }
  return 1;
}

void load_obj_file_data(char *file_name) {
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
      array_push(mesh.vertices, point);
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
      array_push(mesh.faces, face);
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

void free_resources(void) {
  free(color_buffer);
  free(z_buffer);
  upng_free(png_texture);
  array_free(mesh.faces);
  array_free(mesh.vertices);
}

vec3_t cube_vertices[N_CUBE_VERTICES] = {
    {.x = -1, .y = -1, .z = -1}, // 1
    {.x = -1, .y = 1, .z = -1},  // 2
    {.x = 1, .y = 1, .z = -1},   // 3
    {.x = 1, .y = -1, .z = -1},  // 4
    {.x = 1, .y = 1, .z = 1},    // 5
    {.x = 1, .y = -1, .z = 1},   // 6
    {.x = -1, .y = 1, .z = 1},   // 7
    {.x = -1, .y = -1, .z = 1}   // 8
};

face_t cube_faces[N_CUBE_FACES] = {
    // front
    {.a = 1,
     .b = 2,
     .c = 3,
     .a_uv = {0, 1},
     .b_uv = {0, 0},
     .c_uv = {1, 0},
     .color = 0xFFFFFFFF},
    {.a = 1,
     .b = 3,
     .c = 4,
     .a_uv = {0, 1},
     .b_uv = {1, 0},
     .c_uv = {1, 1},
     .color = 0xFFFFFFFF},
    // right
    {.a = 4,
     .b = 3,
     .c = 5,
     .a_uv = {0, 1},
     .b_uv = {0, 0},
     .c_uv = {1, 0},
     .color = 0xFFFFFFFF},
    {.a = 4,
     .b = 5,
     .c = 6,
     .a_uv = {0, 1},
     .b_uv = {1, 0},
     .c_uv = {1, 1},
     .color = 0xFFFFFFFF},
    // back
    {.a = 6,
     .b = 5,
     .c = 7,
     .a_uv = {0, 1},
     .b_uv = {0, 0},
     .c_uv = {1, 0},
     .color = 0xFFFFFFFF},
    {.a = 6,
     .b = 7,
     .c = 8,
     .a_uv = {0, 1},
     .b_uv = {1, 0},
     .c_uv = {1, 1},
     .color = 0xFFFFFFFF},
    // left
    {.a = 8,
     .b = 7,
     .c = 2,
     .a_uv = {0, 1},
     .b_uv = {0, 0},
     .c_uv = {1, 0},
     .color = 0xFFFFFFFF},
    {.a = 8,
     .b = 2,
     .c = 1,
     .a_uv = {0, 1},
     .b_uv = {1, 0},
     .c_uv = {1, 1},
     .color = 0xFFFFFFFF},
    // top
    {.a = 2,
     .b = 7,
     .c = 5,
     .a_uv = {0, 1},
     .b_uv = {0, 0},
     .c_uv = {1, 0},
     .color = 0xFFFFFFFF},
    {.a = 2,
     .b = 5,
     .c = 3,
     .a_uv = {0, 1},
     .b_uv = {1, 0},
     .c_uv = {1, 1},
     .color = 0xFFFFFFFF},
    // bottom
    {.a = 6,
     .b = 8,
     .c = 1,
     .a_uv = {0, 0},
     .b_uv = {0, 1},
     .c_uv = {1, 1},
     .color = 0xFFFFFFFF},
    {.a = 6,
     .b = 1,
     .c = 4,
     .a_uv = {0, 0},
     .b_uv = {1, 1},
     .c_uv = {1, 0},
     .color = 0xFFFFFFFF}};
