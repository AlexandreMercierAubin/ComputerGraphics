#pragma once
#include "SDL2/SDL.h"

class Renderer
{
public:
	void setupRenderer(SDL_Window * window, SDL_Surface* screenSurface);
	void drawRenderer();
	void deleteRenderer();
private:
	SDL_Window * window;
	SDL_Surface* screenSurface;
	SDL_Renderer *sdlRenderer;
};
