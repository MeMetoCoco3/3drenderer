#include "sort.h"
#include "triangle.h"

void swap_int(int *a, int *b) {
  int tmp = *a;
  *a = *b;
  *b = tmp;
}

void swap_f(float *a, float *b) {
  float tmp = *a;
  *a = *b;
  *b = tmp;
}

void swap_triangle(triangle_t *a, triangle_t *b) {
  triangle_t tmp = *a;
  *a = *b;
  *b = tmp;
}

// void quick_sort(triangle_t *triangles, int low, int high) {
//   if (low < high) {
//     int p = partition(triangles, low, high);
//     quick_sort(triangles, low, p - 1);
//     quick_sort(triangles, p + 1, high);
//   }
// }
//
// int partition(triangle_t *triangles, int low, int high) {
//   triangle_t pivot = triangles[high];
//
//   int i = low;
//   for (int j = low; j < high; j++) {
//     if (triangles[j].avg_depth > pivot.avg_depth) {
//       swap_triangle(&triangles[i], &triangles[j]);
//       i++;
//     }
//   }
//   swap_triangle(&triangles[i], &triangles[high]);
//   return i;
// }
//
// void print_arr(int *arr, int array_length) {
//   printf("{ ");
//   for (int i = 0; i < array_length; i++) {
//     printf("%d ", arr[i]);
//   }
//
//   printf("}\n");
// }
