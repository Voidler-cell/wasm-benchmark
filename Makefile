# MISYS make: mingw32-make

builddir:
	mkdir -p build

native: builddir benchmark.cpp
	clang++ -O3 benchmark.cpp -o ./build/benchmark-native

wasm: builddir benchmark.cpp
	clang++ -O3 --target=wasm32 --no-standard-libraries -Wl,--no-entry -Wl,--allow-undefined -Wl,--export=c_benchmark -Wl,--export=js_benchmark -o ./build/benchmark-wasm.wasm benchmark.cpp -DPLATFORM_WEB

all: native wasm

run_native: native
	./build/benchmark-native.exe

clean: builddir
	rm -fr ./build
