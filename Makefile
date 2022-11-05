build:
	emcc ./src/*.c ./lib/*.c -s USE_SDL=2 --preload-file ./images -o raycasting.html;

run:
	./raycasting;

clean:
	rm raycasting*;
