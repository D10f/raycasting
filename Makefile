build:
	gcc -std=c99 ./src/*.c -lSDL2 -lm -o raycasting;

run:
	./raycasting;

clean:
	rm raycasting;
