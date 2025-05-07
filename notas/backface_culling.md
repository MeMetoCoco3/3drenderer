### Backface culling
Es una tecnica de renderizacion de graficos que decide que triangulos renderizar en funcion de si
estos estan cara a la camara.

Solo si el angulo entre la camara y el normal de la cara es menor de 90 grados, mostraremos la cara.

El algoritmo para decidir si tenemos que culear una cara o no es:

1. Encontrar vectores (B-A) y (C-A).
2. Sacar su Cross product para encontrar la normal de la cara.
3. Sacamos el camara ray que apunta a A, que es (A - CameraPosition).
4. Sacamos el dot product la normal de A y de el camara ray.
5. Le culeamos si es <= 0.
