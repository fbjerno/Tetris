#ifndef TETRIS_APPLICATION
#define TETRIS_APPLICATION

#include <SDL_video.h>
#include <SDL_render.h>

typedef struct Application
{
	SDL_Window* window;
	SDL_Renderer* renderer;
    SDL_Rect viewport;
} Application;

extern Application application;

Application InitializeApplication(int width, int height);

#endif
