#pragma once
#include "SDL2/SDL.h"
#include <GL/glew.h>

class Renderer
{
public:
	void setupRenderer(SDL_Window * window, SDL_GLContext *context);
	void drawRenderer();
	void deleteRenderer();
private:
	SDL_Window * window;
	SDL_GLContext gl;
	SDL_Renderer *sdlRenderer;
};
