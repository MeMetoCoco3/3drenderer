### Matrix projection.
Matrix projection es el proceso mediante el cual modificamos un set de cordenadas en 3D de tal forma que consigamos los siguientes resultados (despues del perspective divide!):
1. Normalizar X para conseguir un valor entre -1 y 1, basandonos en el aspect-ratio y FOV.
1. Normalizar X para conseguir un valor entre -1 y 1, basandonos en el FOV.
1. Normalizar X para conseguir un valor entre 0 o (znear) y 1 o (zfar).
Podemos visualizarlo como, el cono que se forma por la perspectiva de la camara , pero transformado en un cubo, haciendo que los objetos cercanos a la camara se expandan y los otros se contraigan.
Tras estos resultados, aplicaremos perspective divide, que es la division del resultado entre el calor de Z antes de la conversion.

Vamos por partes:
#### Aspect Ratio
Escalamos X (ancho) en funcion de Y(alto). Esto se traduce en que si lo aplicamos los objetos no apareceran alargados en monitores anchos.

#### FOV
El triangulo formado por la projeccion de Y sobre la pantalla. Forma dos triangulos uno corta donde Yp y otro donde Y. Sabemos que Yp = Y/distance_camera_to_Y.
Este ratio entre Y/Distance es el ratio entre el opuesto/adjacente, que equivale a la tangente del angulo(FOV)/2.

El caso es que, cuanto mayor FOV usemos, los objetos deben parecer mas pequeños, lo que significa que no usaremos tan(FOV/2), usaremos su inversa: 1/tan(FOV/2).

Esto se lo aplicaremos tanto a X como a Y, ya que FOV determina los objetos que vemos en ambos ejes..

#### Depth
Tenemos que normalizar los valores entre 0 y 1.
Cuando miramos formamos un cono que empieza desde nuestro ojo hacia delante. El ancho de ese cono son las medidas importantes, no la distancia de la camara al objeto. Sabiendo esto, definiremos dos constantes que seran el largo:
- zfar: Profundidad maxima de rendeRIZZado (ancho de cono). 
- znear: Distancia minima de renderizado (ancho de cono).

Definiremos entonces lambda, como zfar/(zfar - znear). Pero aparte de esto debemos aplicar un valor por el offset que es znear, por ello, a lambda le restaremos lambda * znear.

### Resultado:
a = window_height/window_width;
f = 1/tan(FOV/2);
l = zfar/zfar-znear

x -> afx
y -> fy
z -> lz - lznear


### PORQUE NO HACEMOS DIVISION DIVIDE DIRECTAMENTE EN LA MATRIZ
Pues porque se suele hacer clipping antes de aplicar division divide. 
Clipping es el processo mediante el cual decimos que objetos salen en escena y cuales no.
