## Line algorithms
### Definiciones
-  Discrete Raster: Es la malla con celdas numeradas que tenemos que colorear para formar lineas.

### Algoritmos mas populares
#### DDA (digital diferential analizer)
Es un poco mas lento, pero mas facil de entender que Bresenham.
En resumen es: 
0- Calculamos la distancia lateral que es igual a max((x1-x0), (y1-y0)). 
1- Calculamos el incremento de X e Y a lo largo de la distancia lateral.
2- Incrementamos current_x y current_y y redondeamos.
3- Repetimos hasta que hayamos recorrido la diferencia lateral.
```c
void draw_line(int x0, int y0, int x1, int y1){
    int delta_x = x1-x0;
    int delta_y = y1-y0;

    int side_length = abs(delta_x) >= abs(delta_y) ? delta_x : delta_y;

    float x_inc = delta_x / (float) side_length;
    float y_inc = delta_y / (float) side_length;

    float current_x = x0;
    float current_y = y0;

    for (int i = 0; i<=side_length; i++){
        draw_pixel(round(current_x), round(current_y));
        current_x += x_inc;
        current_y += y_inc;
    }
}

```

#### Bresenham

