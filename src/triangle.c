#include "triangle.h"
#include "display.h"
#include "sort.h"

void draw_filled_triangle(int x0, int y0, int x1, int y1, int x2, int y2,
                          uint32_t color) {
  // Necesitamos los vertices ordenados en el eje vertical.
  // y0 < y1 < y2
  if (y0 > y1) {
    swap_int(&y0, &y1);
    swap_int(&x0, &x1);
  }
  if (y1 > y2) {
    swap_int(&y1, &y2);
    swap_int(&x1, &x2);
  }
  if (y0 > y1) {
    swap_int(&y0, &y1);
    swap_int(&x0, &x1);
  }
  if (y1 == y2) {
    fill_flat_bottom_triangle(x0, y0, x1, y1, x2, y2, color);
  } else if (y0 == y1) {
    fill_flat_top_triangle(x0, y0, x1, y1, x2, y2, color);
  } else {

    int my = y1;
    // int mx = ((float)((x2 - x0) * (y1 - y0)) / (float)(y2 - y0)) + x0;
    float alpha = (float)(y1 - y0) / (float)(y2 - y0);
    float mx = x0 + (x2 - x0) * alpha;
    fill_flat_bottom_triangle(x0, y0, x1, y1, mx, my, color);
    fill_flat_top_triangle(x1, y1, mx, my, x2, y2, color);
  }
}

void draw_textured_triangle(int x0, int y0, float u0, float v0, int x1, int y1,
                            float u1, float v1, int x2, int y2, float u2,
                            float v2, uint32_t *texture) {
  // Necesitamos los vertices ordenados en el eje vertical.
  // y0 < y1 < y2
  if (y0 > y1) {
    swap_int(&y0, &y1);
    swap_int(&x0, &x1);
    swap_f(&v0, &v1);
    swap_f(&v0, &v1);
  }
  if (y1 > y2) {
    swap_int(&y1, &y2);
    swap_int(&x1, &x2);
    swap_f(&v1, &v2);
    swap_f(&v1, &v2);
  }
  if (y0 > y1) {
    swap_int(&y0, &y1);
    swap_int(&x0, &x1);
    swap_f(&v0, &v1);
    swap_f(&v0, &v1);
  }

  float inverse_slope_1 = 0;
  float inverse_slope_2 = 0;

  // Dibujamos el flat bottom triangle.
  if (y1 - y0 != 0) {
    inverse_slope_1 = (float)(x1 - x0) / abs(y1 - y0);
  }
  if (y2 - y0 != 0) {
    inverse_slope_2 = (float)(x2 - x0) / abs(y2 - y0);
  }

  if (y1 - y0 != 0) {
    for (int y = y0; y <= y1; y++) {
      int x_start = x1 + (y - y1) * inverse_slope_1;
      int x_end = x0 + (y - y0) * inverse_slope_2;

      // forzaremos que x_start siempre sea el valor menor ya que dibujamos de
      // izquierda a derecha.
      if (x_start > x_end) {
        swap_int(&x_start, &x_end);
      }
      for (int x = x_start; x < x_end; x++) {
        draw_pixel(x, y, (x % 2 == 0 && y % 2 == 0) ? 0xFFFF00FF : 0xFF000000);
      }
    }
  }

  // Dibujamos el flat top triangle.
  if (y2 - y1 != 0) {
    inverse_slope_1 = (float)(x2 - x1) / abs(y2 - y1);
  }
  if (y2 - y0 != 0) {
    inverse_slope_2 = (float)(x2 - x0) / abs(y2 - y0);
  }

  if (y2 - y1 != 0) {
    for (int y = y1; y <= y2; y++) {
      int x_start = x1 + (y - y1) * inverse_slope_1;
      int x_end = x0 + (y - y0) * inverse_slope_2;

      // forzaremos que x_start siempre sea el valor menor ya que dibujamos de
      // izquierda a derecha.
      if (x_start > x_end) {
        swap_int(&x_start, &x_end);
      }
      for (int x = x_start; x < x_end; x++) {
        draw_pixel(x, y, (x % 2 == 0 && y % 2 == 0) ? 0xFFFF00FF : 0xFF000000);
      }
    }
  }
}

void fill_flat_bottom_triangle(int x0, int y0, int x1, int y1, int mx, int my,
                               uint32_t color) {

  // Inverse Slope:
  // Teniendo en cuenta que y tiene un incremento de 1 por 1,
  // cuanto incrementamos en x?
  float slope_inverse_1 = (float)(x1 - x0) / (y1 - y0);
  float slope_inverse_2 = (float)(mx - x0) / (my - y0);
  float start_x = x0;
  float end_x = x0;

  for (int i = y0; i <= my; i++) {
    draw_line(start_x, i, end_x, i, color);

    start_x += slope_inverse_1;
    end_x += slope_inverse_2;
  }
}

void fill_flat_top_triangle(int x1, int y1, int mx, int my, int x2, int y2,
                            uint32_t color) {

  // Inverse Slope:
  // Teniendo en cuenta que y tiene un incremento de 1 por 1,
  // cuanto incrementamos en x?
  float slope_inverse_1 = (float)(x2 - x1) / (y2 - y1);
  float slope_inverse_2 = (float)(x2 - mx) / (y2 - my);

  float start_x = x2;
  float end_x = x2;
  for (int y = y2; y >= my; y--) {
    draw_line(start_x, y, end_x, y, color);

    start_x -= slope_inverse_1;
    end_x -= slope_inverse_2;
  }
}
