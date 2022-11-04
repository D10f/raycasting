build:
	gcc -std=c99 ./src/*.c ./lib/*.c -lSDL2 -lm -o raycasting;

run:
	./raycasting;

clean:
	rm raycasting;
