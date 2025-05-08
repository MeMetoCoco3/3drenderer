#include "display.h"
#include <stdint.h>

SDL_Window *window = NULL;
SDL_Renderer *renderer = NULL;

uint32_t *color_buffer = NULL;
SDL_Texture *color_buffer_texture = NULL;

int window_height = 600;
int window_width = 800;

bool init_window(void) {
  if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {

    fprintf(stderr, "Error initializing SDL.\n");
    return false;
  }

  // Query SDL to know size of display.
  SDL_DisplayMode display_mode;
  SDL_GetCurrentDisplayMode(0, &display_mode);
  window_height = display_mode.h;
  window_width = display_mode.w;

  window =
      SDL_CreateWindow(NULL, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                       window_width, window_height, SDL_WINDOW_BORDERLESS);
  if (!window) {
    fprintf(stderr, "Error initializing Window.\n");
    return false;
  }
  // El segundo argumento define el device context, si es -1, pilla el primero
  // que pueda.
  renderer = SDL_CreateRenderer(window, -1, 0);
  if (!renderer) {
    fprintf(stderr, "Error initializing Renderer.\n");
    return false;
  }
  // Set Fullscreen.
  SDL_SetWindowFullscreen(window, SDL_WINDOW_FULLSCREEN);
  return true;
}

void draw_pixel(int x, int y, uint32_t color) {
  if (x < window_width && x >= 0 && y < window_height && y >= 0) {
    color_buffer[x + y * window_width] = color;
  }
}

// DDA ALGORITHM
void draw_line(int x0, int y0, int x1, int y1, u_int32_t color) {
  int delta_x = x1 - x0;
  int delta_y = y1 - y0;

  int side_length = abs(delta_x) >= abs(delta_y) ? abs(delta_x) : abs(delta_y);

  float x_inc = delta_x / (float)side_length;
  float y_inc = delta_y / (float)side_length;

  float current_x = x0;
  float current_y = y0;

  for (int i = 0; i <= side_length; i++) {
    draw_pixel(round(current_x), round(current_y), color);
    current_x += x_inc;
    current_y += y_inc;
  }
}

void draw_triangle(triangle_t triangle, u_int32_t color) {
  draw_line(triangle.points[0].x, triangle.points[0].y, triangle.points[1].x,
            triangle.points[1].y, color);
  draw_line(triangle.points[1].x, triangle.points[1].y, triangle.points[2].x,
            triangle.points[2].y, color);
  draw_line(triangle.points[2].x, triangle.points[2].y, triangle.points[0].x,
            triangle.points[0].y, color);
}

/* FROM EXERCICE DRAWING A BACKGROUND GRID*/
void draw_grid(uint32_t color) {
  for (int y = 0; y < window_height; y++) {
    for (int x = 0; x < window_width; x++)
      if (y % 10 == 0 || x % 10 == 0) {
        draw_pixel(x, y, color);
      }
  }
}

void draw_grid_points(uint32_t color) {
  for (int y = 0; y < window_height; y += 10) {
    for (int x = 0; x < window_width; x += 10) {
      draw_pixel(x, y, color);
    }
  }
}

/* FROM EXERCICE DRAWING A RECTANGLE*/
void draw_rectangle_lines(int vx, int vy, int width, int height,
                          uint32_t color) {
  for (int x = vx; x < width + vx; x++) {
    color_buffer[x + window_width * vy] = color;
    color_buffer[x + window_width * (vy + height)] = color;
  }
  for (int y = vy; y < height + vy; y++) {
    color_buffer[y * window_width + vx] = color;
    color_buffer[y * window_width + (vx + width)] = color;
  }
}

/* FROM EXERCICE DRAWING A RECTANGLE*/
void draw_rectangle(int vx, int vy, int width, int height, uint32_t color) {
  for (int y = 0; y < height; y++) {
    for (int x = 0; x < width; x++) {
      int current_x = x + vx;
      int current_y = y + vy;
      draw_pixel(current_x, current_y, color);
      /* color_buffer[current_y * window_width + current_x] = color; */
    }
  }
}

void render_color_buffer(void) {
  // Updates texture with the values from color buffer.
  SDL_UpdateTexture(color_buffer_texture, NULL, color_buffer,
                    (int)sizeof(uint32_t) * window_width);
  // Renders texture on device.
  SDL_RenderCopy(renderer, color_buffer_texture, NULL, NULL);
}
void clear_color_buffer(uint32_t color) {
  for (int y = 0; y < window_height; y++) {
    for (int x = 0; x < window_width; x++)
      color_buffer[y * window_width + x] = color;
  }
}

void destroy_window(void) {
  printf("Color buffer freed.\n");

  SDL_DestroyRenderer(renderer);
  printf("Renderer freed.\n");

  SDL_DestroyWindow(window);
  printf("Window freed.\n");

  SDL_Quit();
  printf("SDL QUIT.\n");
}
