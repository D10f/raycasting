build:
	emcc ./src/*.c ./lib/*.c -s USE_SDL=2 -s WASM=1 --preload-file ./images -o raycasting.js;

run:
	./raycasting;

clean:
	rm raycasting*;
