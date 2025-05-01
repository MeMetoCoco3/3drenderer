### Headers e implementation files
Se utilizan para organizar nuestro codigo y ganar calidad de vida. Cuando intentamos compilar, el
parser chekea que nuestras funciones tienen el mismo prototipo que las declaradas en el header, el
linker se encarga de unir ambos object files una vez compilados.
Es posible que, por cosas de la vida, se importen multiples veces los prototipos y variables u otros
archivos que decicamos incluir en nuestro codigo, por ejemplo stdbool., stdint.h, para evitar esto 
debemos usar el macro ifndef.:
```c
#ifndef RANDOMLABEL
#define RANDOMLABEL
...
#endif
```
Los header files tienen unicamente declaraciones de variables(no definicion) y prototipos de funciones.
Los implementation tienen la implementacion de las funciones.

El compilador da warnings si delaramos variables en nuestro header file sin usar la keyword extern,
que basicamente indica que la definicion es externa.

```c
extern SDL_Window* window;
```


