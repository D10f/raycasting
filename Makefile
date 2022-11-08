build:
	emcc ./src/*.c ./lib/*.c -s USE_SDL=2 -s WASM=1 -s ENVIRONMENT=web --preload-file ./images -o public/raycasting.js;

run:
	emrun --port 8080 --hostname localhost --no_browser public/index.html

clean:
	rm public/raycasting*;
