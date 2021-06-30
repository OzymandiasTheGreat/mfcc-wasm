emcc \
	-s WASM=1 \
	-s STANDALONE_WASM=1 \
	--no-entry \
	-Oz \
	-D NDEBUG \
	-D kiss_fft_scalar=double \
	./src/*.c \
	./kissfft/*.c \
	./libmfcc/libmfcc.c \
	-s EXPORTED_FUNCTIONS="['_malloc', '_free']" \
	-o ./dist/mfcc.wasm
base64 ./dist/mfcc.wasm > base64.txt
