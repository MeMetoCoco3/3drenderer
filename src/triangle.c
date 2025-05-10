#include "triangle.h"
#include "display.h"
#include "sort.h"

void draw_filled_triangle(int x0, int y0, int x1, int y1, int x2, int y2,
                          uint32_t color) {
  if (y0 > y1) {
    swap(&y0, &y1);
    swap(&x0, &x1);
  }
  if (y1 > y2) {
    swap(&y1, &y2);
    swap(&x1, &x2);
  }
  if (y0 > y1) {
    swap(&y0, &y1);
    swap(&x0, &x1);
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

void fill_flat_bottom_triangle(int x0, int y0, int x1, int y1, int mx, int my,
                               uint32_t color) {

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
