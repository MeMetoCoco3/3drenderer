#ifndef DISPLAY_H
#define DISPLAY_H

#include <SDL2/SDL.h>
#include <stdbool.h>
#include <stdint.h>

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
void draw_rectangle_lines(int vx, int vy, int width, int height,
                          uint32_t color);
void draw_grid(void);
void draw_rectangle(int vx, int vy, int width, int height, uint32_t color);
#endif
