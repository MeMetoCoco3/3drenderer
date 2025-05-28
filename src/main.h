#ifndef MAIN_H
#define MAIN_H

#include "mesh.h"
void setup(void);
void get_input(void);
void update(void);
void render_color_buffer(void);
void render(void);
void free_resources(void);
void process_graphics_pipeline_stages(mesh_t *mesh);
#endif
