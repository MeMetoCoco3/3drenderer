#ifndef DISPLAY_H
#define DISPLAY_H

#include "triangle.h"
#include <SDL2/SDL.h>
#include <stdbool.h>
#include <stdint.h>

#define FPS 30
// Frame target time es el numero de milisegundos que cada frame esta dibujado
// en pantalla.
#define FRAME_TARGET_TIME (1000 / FPS)

enum RENDERING_MODES {
  RM_WIREFRAME,
  RM_WIREFRAME_LINES,
  RM_COLORED,
  RM_COLORED_LINES
};

enum BACKFACE_CULLING { BACKFACE_CULLING_ON, BACKFACE_CULLING_OF };

typedef struct {
  enum RENDERING_MODES rm;
  enum BACKFACE_CULLING bc;
} rendering_data_t;

extern SDL_Window *window;
extern SDL_Renderer *renderer;

extern uint32_t *color_buffer;
extern SDL_Texture *color_buffer_texture;

extern int window_height;
extern int window_width;

bool init_window(void);
void setup(void);
void get_input(void);
void update(void);
void render_color_buffer(void);
void render(void);
void destroy_window(void);
void clear_color_buffer(uint32_t color);

void draw_pixel(int x, int y, uint32_t color);
void draw_line(int x0, int y0, int x1, int y1, u_int32_t color);
void draw_triangle(triangle_t triangle, u_int32_t color);
void draw_rectangle_lines(int vx, int vy, int width, int height,
                          uint32_t color);
void draw_grid(uint32_t);
void draw_grid_points(uint32_t);
void draw_rectangle(int vx, int vy, int width, int height, uint32_t color);
#endif
