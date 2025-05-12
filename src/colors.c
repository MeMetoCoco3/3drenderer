#include "colors.h"

uint32_t light_apply_intensity(uint32_t original_color, float factor) {
  uint32_t a = (original_color >> 24) & 0xFF;
  uint32_t r = (original_color >> 16) & 0xFF;
  uint32_t g = (original_color >> 8) & 0xFF;
  uint32_t b = original_color & 0xFF;

  // Apply intensity factor with clamping to [0, 255]
  r = (uint32_t)(r * factor);
  g = (uint32_t)(g * factor);
  b = (uint32_t)(b * factor);

  if (r > 255)
    r = 255;
  if ((int)r < 0)
    r = 0;
  if (g > 255)
    g = 255;
  if ((int)g < 0)
    g = 0;
  if (b > 255)
    b = 255;
  if ((int)b < 0)
    b = 0;

  uint32_t new_color = (a << 24) | (r << 16) | (g << 8) | b;

  return new_color;
}
