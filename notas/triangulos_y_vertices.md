## Triangulos y vertices
### Definitions
-  Mesh: Conjunto de triangulos que, dibujados en un espacio en tres dimensiones, se ven como un objeto solido.
-  Triangulo: Figura definida por 3 vertices. Tambien nos referimos a ellos como faces.
-  Vertice: Punto definido por 3 valores; X, Y y Z.

### Notes
#### Orden de vertices
Cuando definimos un objeto, tenemos que dibujar cada triangulo, o face, haciendo referencia a los vertices del objeto.
```c

vec3_t cube_points[8] = {
    {.x = -1, .y = -1, .z = -1},
    {.x = -1, .y = 1, .z = -1},
    {.x = 1, .y = 1, .z = -1},
    {.x = 1, .y = -1, .z = -1},
    {.x = 1, .y = 1, .z = 1},
    {.x = 1, .y = -1, .z = 1},
    {.x = -1, .y = 1, .z = 1},
    {.x = -1, .y = -1, .z = -}
};

typedef struct{
    int a, b, c;
} face_t;

face_t cube_faces[12] = {
    // front
    {.a = 1, .b = 2, .c = 3},
    {.a = 1, .b = 3, .c = 4},
    // right
    {.a = 4, .b = 3, .c = 5},
    {.a = 4, .b = 5, .c = 6},
    // back
    {.a = 6, .b = 5, .c = 7},
    {.a = 6, .b = 7, .c = 8},
    // left
    {.a = 8, .b = 7, .c = 2},
    {.a = 8, .b = 2, .c = 1},
    // top
    {.a = 2, .b = 7, .c = 5},
    {.a = 2, .b = 5, .c = 3},
    // bottom
    {.a = 6, .b = 8, .c = 1},
    {.a = 6, .b = 1, .c = 4},
}
```
El orden de los vertices es importante, ya que si escogemos el orden clockwise, la cara estara apuntando hacia fuera.
Si escogemos el orden contraclockwise, la cara estara apuntando hacia dentro.
```c
face_t cube_faces[12] = {
    // front clockwise
    {.a = 1, .b = 2, .c = 3},
    {.a = 1, .b = 3, .c = 4},

    // front contraclockwise
    {.a = 1, .b = 3, .c = 2},
    {.a = 1, .b = 4, .c = 3},
```
