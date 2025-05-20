### Look at function
Es la transformacion que aplicaremos a nuestra world matrix, para transportar la escena a camera/view space.
Esta transformacion es el resultado de translation y rotation.
La translation mueve los objetos de la escena a su nueva posicion, aplicando el vector -eye, ya que imaginate que tenemos eye en (2, 2), pues tenemos que transformarlo a (0, 0) y eso se hace aplicando la transformacion de -(2, 2).
La rotacion cambia el eje de cordenadas a uno donde eye es el centro. Se aplica forward(z), right(x) y up(y). Pero para que todo vaya bien, hay que invertir la matriz, para que el resultado este apuntando a la camara, y no en frente. 


### Orthogonal matrix
Una matriz es ortogonal cuando sus vectores son unit vectors. Y son mutually perpendicular, lo que quiere decir que el dot product entre las rows es 0.
Lo que estamos haciendo nosotros es usar el eje de coordenadas de la camara, usando los 3 axis que son perpendiculares entre si, y usando los valores normalizados.

### Invertir una matrix
M * 1/M = IdentityMatrix
En nuestro caso, como es orthogonal, vamos a transformar las columnas en rows y au!

#### Referencias
Look At transformations: https://courses.pikuma.com/courses/take/learn-computer-graphics-programming/lessons/12514590-look-at-transformations

