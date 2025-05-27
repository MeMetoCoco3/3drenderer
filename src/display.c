#include "display.h"
#include <stdint.h>

static SDL_Window *window = NULL;
static SDL_Renderer *renderer = NULL;

static uint32_t *color_buffer = NULL;
static float *z_buffer = NULL;
static SDL_Texture *color_buffer_texture = NULL;

static int window_height = 200;
static int window_width = 320;

static rendering_data_t rendering_data = {0, 0, 0};

int get_window_width(void) { return window_width; }
int get_window_height(void) { return window_height; }
bool init_window(void) {

  // Iniciamos la libreria.
  if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
    fprintf(stderr, "Error initializing SDL.\n");
    return false;
  }

  // Preguntamos a SDL cual es el tamaño de nuestro monitor.
  SDL_DisplayMode display_mode;
  SDL_GetCurrentDisplayMode(0, &display_mode);
  window_width = display_mode.w;
  window_height = display_mode.h;
  // int full_screen_w = display_mode.w;
  // int full_screen_h = display_mode.h;

  // Creamos una ventana.
  window =
      SDL_CreateWindow(NULL, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                       window_width, window_height, SDL_WINDOW_BORDERLESS);

  if (!window) {
    fprintf(stderr, "Error initializing Window.\n");
    return false;
  }

  // Creamos un renderer con (window, renderer_driver, flags).
  renderer = SDL_CreateRenderer(window, -1, 0);
  if (!renderer) {
    fprintf(stderr, "Error initializing Renderer.\n");
    return false;
  }

  // WARN: CHECK THIS
  // Real fullscreen.
  SDL_SetWindowFullscreen(window, SDL_WINDOW_FULLSCREEN);

  // Creamos un color_buffer con el tamaño de W*H*sizeof(pixel).
  color_buffer =
      (uint32_t *)malloc(sizeof(uint32_t) * window_width * window_height);
  z_buffer = (float *)malloc(sizeof(float) * window_width * window_height);
  // Creamos una textura donde copiaremos el color buffer.
  // Esta textura sera interpretada por el renderer.
  // Pixel Format = AARRGGBB;
  // Lo cambie ya que las texturas estan codificadas en RGBA format.
  // Text Access straming = Recurriremos a este bloque de memoria
  // constantemente.
  color_buffer_texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA32,
                                           SDL_TEXTUREACCESS_STREAMING,
                                           window_width, window_height);

  return true;
}

void set_render_method(enum RENDERING_MODES t) { rendering_data.rm = t; }
void set_cull_method(enum BACKFACE_CULLING t) { rendering_data.bc = t; };
void set_light(enum LIGHTS t) { rendering_data.l = t; };
bool is_cull_backface(void) {
  return rendering_data.bc == BACKFACE_CULLING_ON;
};
bool is_lights_on(void) { return rendering_data.l == LIGHTS_ON; };

bool should_render_filled_triangles(void) {
  return (rendering_data.rm == RM_COLORED ||
          rendering_data.rm == RM_COLORED_LINES);
}

bool should_render_vertex(void) {
  return (rendering_data.rm == RM_WIREFRAME_VERTEX);
}

bool should_render_wireframe(void) {
  return (rendering_data.rm == RM_COLORED_LINES ||
          rendering_data.rm == RM_WIREFRAME ||
          rendering_data.rm == RM_WIREFRAME_VERTEX ||
          rendering_data.rm == RM_TEXTURED_WIRE);
}

bool should_render_textured_triangles(void) {
  return (rendering_data.rm == RM_TEXTURED ||
          rendering_data.rm == RM_TEXTURED_WIRE);
}

void draw_pixel(int x, int y, uint32_t color) {
  if (x < 0 || x >= window_width || y < 0 || y >= window_height) {
    return;
  }
  color_buffer[x + y * window_width] = color;
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
  SDL_RenderPresent(renderer);
}
void clear_color_buffer(uint32_t color) {
  for (int i = 0; i < window_height * window_width; i++) {
    color_buffer[i] = color;
  }
}

float get_zbuffer_at(int x, int y) {

  if (x < 0 || x >= window_width || y < 0 || y >= window_height) {
    return 1.0;
  }
  return z_buffer[x + (y * window_width)];
}
void update_zbuffer_at(int x, int y, float value) {
  if (x < 0 || x >= window_width || y < 0 || y >= window_height) {
    return;
  }
  z_buffer[x + (y * window_width)] = value;
}
void clear_z_buffer(void) {
  for (int i = 0; i < window_height * window_width; i++) {
    z_buffer[i] = 1.0;
  }
}

void destroy_window(void) {
  free(color_buffer);
  free(z_buffer);
  printf("Color buffer freed.\n");

  SDL_DestroyRenderer(renderer);
  printf("Renderer freed.\n");

  SDL_DestroyWindow(window);
  printf("Window freed.\n");

  SDL_Quit();
  printf("SDL QUIT.\n");
}
