### Deltatime
Cuando creamos un loop en nuestro codigo el ordenador (o mas bien, la CPU) intenta hacer las tareas tan rapido como puede.

-  FPS: Numero de veces que nuestro loop ocurre por segundo.
-  Frame target time: Numero de milisegundos que cada frame esta dibujado. Es equivalente a (1000 / FPS)
-  SDL_GetTicks(): Cuando iniciamos SDL, esta funcion se pone a contar el numero de ticks de la CPU, esto nos 
    permite medir el tiempo que un frame tarda en ejecutarse.
-  SDL_TICKS_PASSED(a, b): Si b-a <=0 entonces true.

```C
  while (!SDL_TICKS_PASSED(SDL_GetTicks(),
                           previous_frame_time + FRAME_TARGET_TIME))
    ;
```

Pero esta version es un poco naive, ya que cuando compilamos nuestro programa, el SO ve el while loop y hace focus en esa tarea,
ya que el no diferencia en base al contenido del while, asi que tenemos que encontrar otra forma de hacerlo, para que el SO sepa
que puede compartir su capacidad de procesamiento con otras tareas. 
    Esta forma es SDL_Delay(ms), que utiliza un timer del SO para saber que lo unico que tienes que hacer es esperar, y mientras,
utilizar la CPU para otras cosas

```C
int time_to_wait = FRAME_TARGET_TIME - (SDL_GetTicks() - previous_frame_time);
if (time_to_wait > 0 && time_to_wait <= FRAME_TARGET_TIME){
    SDL_Delay(time_to_wait);
}
previous_frame_time = SDL_GetTicks();
```
