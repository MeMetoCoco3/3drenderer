#ifndef SORT_H
#define SORT_H

#include "triangle.h"
#include <stdio.h>
void swap_int(int *a, int *b);
void swap_f(float *a, float *b);
void swap_triangle(triangle_t *a, triangle_t *b);

void quick_sort(triangle_t *triangles, int low, int high);
int partition(triangle_t *triangles, int low, int high);
void print_arr(int *arr, int array_length);
#endif
