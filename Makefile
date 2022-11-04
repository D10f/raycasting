build:
	emcc ./src/*.c ./lib/*.c -s USE_SDL=2 \
	--preload-file ./images/bluestone.png \
	--preload-file ./images/colorstone.png \
	--preload-file ./images/eagle.png \
	--preload-file ./images/graystone.png \
	--preload-file ./images/mossystone.png \
	--preload-file ./images/pikuma.png \
	--preload-file ./images/purplestone.png \
	--preload-file ./images/redbrick.png \
	--preload-file ./images/wood.png \
	-o raycasting.html;

run:
	./raycasting;

clean:
	rm raycasting *.js *.wasm *.html;
