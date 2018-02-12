#pragma once
#include "SDL2/SDL.h"

class Renderer
{
public:
	void setup(SDL_Window * window, SDL_Surface* screenSurface);
	void draw();
	void exit();
private:
	SDL_Window * window;
	SDL_Surface* screenSurface;
	SDL_Renderer *sdlRenderer;
};
