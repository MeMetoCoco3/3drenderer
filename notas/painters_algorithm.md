### Painters algorithm

Es un algoritmo encargado de ordenar las caras de un modelo o escena en funcion la media de la
profundidad de los 3 vertices.

De atras al frente.

#### EL PROBLEMA
Si imaginamos nuestra pantalla como un conjunto de capas, cada cara estaria en una capa, lo que quiere decir que,
si un vertice de una cara, esta mas cerca de la pantalla que el de otra; pero su avg_depth es mayor, ignoraremos 
dicho vertice, y dibujaremos todo el triangulo detras del otro.
