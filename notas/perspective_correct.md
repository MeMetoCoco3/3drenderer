### Perspective correct
Se refiere a la tecnica que aplicamos a nuestras texturas, para que estas sean deformadas por la perspectiva de la imagen.
Una suerte de perspective divide aplicada a la textura dibujada en el triangulo.

Esta deformacion ocurre porque al rotar una cara el centro de la imagen "se desplaza" deformando la imagen que queremos copiar en la cara.

Para solucionarlo tenemos que encontrar la profundidad en el punto a dibujar, lo curioso es que no podemos interpolar Z directamente, ya que no es lineal, lo que si es lineal (x = mz+c) es el calculo de la inversa de Z (1/Z)

Por lo tanto, tendremos que calcular el interpolated_u usando no u0,u1,u2 si no u0/w, u1/w, u0/w, lo mismo en v, y calcularemos tambien el interpoladed_w usando la inversa de w(1/w).
El paso final es interpolated_u/=interpolated_w, lo mismo en v.


###### Referencias
https://s3.amazonaws.com/thinkific/file_uploads/167815/attachments/c06/b72/f58/KokLimLow-PerspectiveCorrectInterpolation.pdf
