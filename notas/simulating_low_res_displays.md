### Simulando LOW RES displays

Podemos cambiar el tamaño de nuestra pantalla, PERO iniciar la ventana de SDL con los valores que SDL_DisplayMode nos de. Esto creara una ventana grande y nos hara trabajar con un color buffer pequeño, EL CASO, SDL_RenderCopy es muy inteligente, y escalara nuestro color buffer para rellenar la pantalla, dando un resultado muy chulo!
