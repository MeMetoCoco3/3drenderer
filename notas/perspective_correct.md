### Perspective correct
Se refiere a la tecnica que aplicamos a nuestras texturas, para que estas sean deformadas por la perspectiva de la imagen.
Una suerte de perspective divide aplicada a la textura dibujada en el triangulo.

Esta deformacion ocurre porque al rotar una cara el centro de la imagen "se desplaza" deformando la imagen que queremos copiar en la cara.

Para solucionarlo tenemos que encontrar la profundidad en el punto a dibujar, lo curioso es que no podemos interpolar Z directamente, ya que no es lineal, lo que si es lineal (x = mz+c) es el calculo de la inversa de Z (1/Z)
