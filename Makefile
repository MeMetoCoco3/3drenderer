build:
	gcc  -std=c99 src/*.c  -lSDL2 -lm -o renderer
d:
	gcc -Wall -Wextra -std=c99 src/*.c  -lSDL2 -lm -o renderer

run:
	./renderer
clean:
	rm renderer
