includes := -I deps/include/SDL2/ -I deps/include/SDL_FontCache
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
	gcc $(files) deps/include/SDL_FontCache/SDL_FontCache.c  -I $(srcdir) $(includes) -o $(out) -L $(libdir) -l SDL2 -l SDL2_image -l SDL2_ttf \
	$(flags)
	cp ./assets/*.png ./bin/assets
