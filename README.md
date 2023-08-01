# 4Coder Community

Welcome to the 4coder community repository.

# Building
To build 4coder you only need a C++ compiler (`cl`, `g++`, `clang++`, `circle (as of verison 198 it can not build the compiler intrisincs)`)
By compiling the 4ed_build.cpp file in the root directory and running the resulting binary (most of the code base uses `char*` for strings in read-only data, to clean up the output we remove those warnings for each compiler.

## Windows
1. Compile 
		` cl /Icode\custom 4ed_build.cpp /Fe4ed_build && .\4ed_build.exe `

## Linux
> tested on Ubuntu 22.04:
1. Get required libraries (apt names):
    - `$ sudo apt install build-essential libx11-dev libxfixes-dev libglx-dev mesa-common-dev libasound2-dev libfreetype-dev libfontconfig-dev`

2. Compile
		`g++     4ed_build.cpp -Icode -Icode/custom  -Wno-write-strings    -o 4ed_build`

	or (sadly it doesn't build with the circle compiler yet)

		`circle  4ed_build.cpp -Icode -Icode/custom --Wno-writable-strings -o 4ed_build `

	or

		`clang++ 4ed_build.cpp -Icode -Icode/custom  -Wno-write-strings -Wno-null-dereference -o 4ed_build `

## Mac (Untested)
1.Compile

	`clang++ 4ed_build.cpp -Icode -Icode/custom  -Wno-write-strings -Wno-null-dereference -o 4ed_build`


# Running the build system
The build system takes as input a few flags `dev` (debug), `opt` (optimized), `package`, `custom=<target>`(as.
The easiest way of getting 4coder is to run

		`4ed_build opt`

this compiles `4ed` (platform layer), `4ed_app.so` (core layer), `custom_4coder.so` (custom layer) and puts the runtime resourses next to the binary (`bindings.4coder`, `config.4coder`, `font` directory, `themes` directory, and some custom layers).
