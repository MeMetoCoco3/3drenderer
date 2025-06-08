### Handedness
Al trabajar en 3D tenemos que declarar nuestro eje de coordenadas, y dependiendo de como trabajemos sera:
- Left Handed: Eje Z es positivo hacia dentro de la pantalla.
- Right Handed: Eje Z es positivo hacia nosotros.
Esta diferencia hace que tengamos que tener cuidado en muchos otros calculos de nuestro proyecto.

#### Direccion de rotacion
Cuando estamos mirando desde 0 hacia el eje que queremos rotar (EJ: +Y)
- Left Handed: Clockwise rotation.
- Right Handed: Counterclockwise rotation.

#### Cross-Product
Las matematicas son agnosticas al eje de coordenadas que estemos utilizando. Lo que significa que si calulamos
el cross-product de una cara que este apuntando a nosotros, nos dara un resultado negativo, lo que significa:
- Left Handed: Todo bien, nuestro Eje Z crece en negativo hacia nosotros, es lo que buscamos.
- Right Handed: Meh, el resultado apunta hacia dentro de la pantalla, y no es lo que buscamos.

El calculo sera siempre el mismo ( cross ((b-a),(c-a)) ), pero el resultado sera diferente y tendremos que
tenerlo cuenta en nuestra interpretacion del resultado.

##### Orden de vertices
Los vertices pueden estar declarados clockwise o counterclowise, esto tambien afectara  a nuestro resultado.
Si es counterclockwise cuando calculemos el RH_Cross este estara apuntando hacia nosotos.

Este esquema muestra como calcular el normal para que apunte hacia arriba respecto a la face.

:| Sistema | OrdenVertices | Cross-Product |
| --------------- | --------------- | --------------- |
| RH | Counter | cross(e1, e2) |
| LH | Clockwise | cross(e1, e2) |
| LH | Counter | -cross(e1, e2) |


#### Z-Buffer
Tendremos en cuenta el EjeZ para medir que pixeles estan en frente.

#### GraphicsAPI
APIs como Vulkan o OpenGL utilizan un sistema Right Handed, mientras que Direct3D utiliza Left Handed.

##### Resources 
https://courses.pikuma.com/courses/take/learn-computer-graphics-programming/lessons/12297168-handedness-orientation
