#ifndef TETRIS_APPLICATION
#define TETRIS_APPLICATION

#include <SDL_video.h>
#include <SDL_render.h>

typedef struct Application
{
	SDL_Window* window;
	SDL_Renderer* renderer;
} Application;

extern Application application;

Application InitializeApplication(int width, int height);

#endif