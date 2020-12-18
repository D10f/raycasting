build:
	gcc -std=c99 ./src/main.c -lSDL2 -lm -o raycasting;

run:
	./raycasting;

clean:
	rm raycasting;
