#ifndef DISPLAY_H
#define DISPLAY_H

#include "triangle.h"
#include <SDL2/SDL.h>
#include <stdbool.h>
#include <stdint.h>

#define FPS 60
// Frame target time es el numero de milisegundos que cada frame esta dibujado
// en pantalla.
#define FRAME_TARGET_TIME (1000 / FPS)

enum RENDERING_MODES {
  RM_WIREFRAME,
  RM_WIREFRAME_VERTEX,
  RM_COLORED,
  RM_COLORED_LINES,
  RM_TEXTURED,
  RM_TEXTURED_WIRE,
};

enum BACKFACE_CULLING { BACKFACE_CULLING_ON, BACKFACE_CULLING_OF };

enum LIGHTS { LIGHTS_ON, LIGHTS_OF };

typedef struct {
  enum RENDERING_MODES rm;
  enum BACKFACE_CULLING bc;
  enum LIGHTS l;
} rendering_data_t;

bool init_window(void);
int get_window_width(void);
int get_window_height(void);
void set_render_method(enum RENDERING_MODES t);
void set_cull_method(enum BACKFACE_CULLING t);
void set_light(enum LIGHTS t);
bool is_cull_backface(void);
bool is_lights_on(void);
bool should_render_filled_triangles(void);
bool should_render_textured_triangles(void);
bool should_render_vertex(void);
bool should_render_wireframe(void);

void destroy_window(void);
void clear_color_buffer(uint32_t color);
void clear_z_buffer(void);
void draw_pixel(int x, int y, uint32_t color);
void draw_line(int x0, int y0, int x1, int y1, u_int32_t color);
void draw_triangle(triangle_t triangle, u_int32_t color);
void draw_rectangle_lines(int vx, int vy, int width, int height,
                          uint32_t color);
void draw_grid(uint32_t);
void draw_grid_points(uint32_t);
void draw_rectangle(int vx, int vy, int width, int height, uint32_t color);

float get_zbuffer_at(int x, int y);
void update_zbuffer_at(int x, int y, float value);
#endif
