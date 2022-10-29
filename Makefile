
includedir := deps/include/*/
libdir := deps/lib/
srcdir := src/
files := src/*.c
flags = -Wall -m64 -mwindows

entry := ./src/Application.c
out := bin/Tetris.exe

all: build

debug: flags += -mconsole -g
debug: build

build:
	gcc $(files) -I $(srcdir) -I $(includedir) -o $(out) -L $(libdir) -l SDL2 -l SDL2_image \
	$(flags)
	cp ./assets/*.png ./bin/assets