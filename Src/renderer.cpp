#include "renderer.h"

void Renderer::setup(SDL_Window * window, SDL_Surface* screenSurface)
{
	this->window = window;
	this->screenSurface = screenSurface;
	sdlRenderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
}

void Renderer::draw()
{

	//Update the surface
	SDL_UpdateWindowSurface(window);

	SDL_SetRenderDrawColor(sdlRenderer, 0, 0, 0, SDL_ALPHA_OPAQUE);

	SDL_RenderClear(sdlRenderer);
	
	SDL_SetRenderDrawColor(sdlRenderer, 255, 255, 255, SDL_ALPHA_OPAQUE);
	SDL_RenderDrawLine(sdlRenderer, 0, 0, screenSurface->w, screenSurface->h);

	SDL_RenderPresent(sdlRenderer);
	


	//Wait two seconds
	SDL_Delay(2000);
}

void Renderer::exit()
{
	SDL_DestroyRenderer(sdlRenderer);
}

