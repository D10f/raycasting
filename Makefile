build:
	emcc ./src/*.c ./lib/*.c -s USE_SDL=2 -lm --preload-file ./images/redbrick.png -o raycasting.html;

run:
	./raycasting;

clean:
	rm raycasting *.js *.wasm *.html;
