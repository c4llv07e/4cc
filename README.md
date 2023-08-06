# 4Coder Community

Welcome to the 4coder community repository.

## Building
To build 4coder you only need a C++ compiler (`cl`, `g++`, `clang++`, `circle (as of verison 198 it can not build the compiler intrisincs)`)
By compiling the 4ed_build.cpp file in the root directory and running the resulting binary (most of the code base uses `char*` for strings in read-only data, to clean up the output we remove those warnings for each compiler).

## Compile
### Windows

	cl /Icode\custom 4ed_build.cpp /Fe4ed_build
   
### Linux
1. Get required libraries (apt names)

	```bash
 	sudo apt install build-essential libx11-dev libxfixes-dev libglx-dev mesa-common-dev libasound2-dev libfreetype-dev libfontconfig-dev
	```
 
2. Compile

        g++     4ed_build.cpp -Icode/custom  -Wno-write-strings    -o 4ed_build

	or (sadly it doesn't build with the circle compiler yet)

		circle  4ed_build.cpp -Icode/custom --Wno-writable-strings -o 4ed_build

	or

		clang++ 4ed_build.cpp -Icode/custom  -Wno-write-strings -Wno-null-dereference -o 4ed_build

### Mac (Untested)

	clang++ 4ed_build.cpp -Icode/custom  -Wno-write-strings -Wno-null-dereference -o 4ed_build


## Running the build system
The build system takes as input a few flags `dev` (debug), `opt` (optimized), `custom=<target>`.
The easiest way of getting 4coder is to run

	4ed_build opt

this compiles `4ed` (platform layer), `4ed_app.so` (core layer), `custom_4coder.so` (custom layer) and puts the runtime resourses next to the binary (`bindings.4coder`, `config.4coder`, `font` directory, `themes` directory, and some custom layers).

### Custom targets
4coder is split between two layer (core and custom). If at build time the `custom=<target>` is specified the build system will build the `<target>/<target>.cpp`. By cloning this repo recursively, some example custom layers will also be cloned and can be built by passing the name of the folder as argument to the build system. Build `4coder_byp` for a vim like experience or build `4coder_fleury` for a emacs like experience. 4coder is emacs like by default, but the 4coder_fleury custom layer has more features then what you get out of the box.
	
	4ed_build opt custom=4coder_byp

 or
 
 	4ed_build opt custom=4coder_fleury

## Getting started on working on a custom layer
There is an articlewriten by MrMixer on the 4coder - HandMade Network:
https://4coder.handmade.network/forums/articles/t/7319-customization_layer_-_getting_started__4coder_4.1_
