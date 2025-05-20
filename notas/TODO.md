1. Smooth shading. (Gouraud Shading / Phong Shading)
***2. Corregir sistema de coordenadas.***



------
Zbuffer: 
1. Declare and initialize  a float buffer.
2. Clear Z buffer.
3. Update Z buffer on texel.
------
Coding look-at function:
1. Declare camara.h/.c
2. Define origin {0,0,0} for camera_ray calculation on backface culling.
3. Define look_at function
4. Declare new proj_matrix to use on matrix_calculations on update
5. Look at 0010, up is 010
6. use it
