build:
	gcc -std=c99 ./src/*.c -lSDL2 -lm -o raycasting;

wasm:
	emcc ./src/*.c \
		-s USE_SDL=2 \
		-s WASM=1 \
		-s ENVIRONMENT=web \
		-s MODULARIZE=1 \
		-s EXPORT_ES6=1 \
		-s EXPORTED_FUNCTIONS="['_main','_toggle_play']" \
		-Os \
		--preload-file ./images \
		-o public/raycasting.js

wasm_run:
	emrun --port 8080 --hostname localhost --no_browser public/index.html

run:
	./raycasting;

clean:
	rm raycasting public/*.{data,js,wasm,html}
