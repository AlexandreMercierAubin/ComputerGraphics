#include "renderer.h"

void Renderer::setupRenderer(SDL_Window * window, SDL_Surface* screenSurface)
{
	this->window = window;
	this->screenSurface = screenSurface;
	sdlRenderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
}

void Renderer::drawRenderer()
{
	SDL_SetRenderDrawColor(sdlRenderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
	SDL_RenderClear(sdlRenderer);

	SDL_SetRenderDrawColor(sdlRenderer, 255, 255, 255, SDL_ALPHA_OPAQUE);
	SDL_RenderDrawLine(sdlRenderer, 0, 0, 400, 400);

	SDL_RenderPresent(sdlRenderer);

	//Wait two seconds
	SDL_Delay(50000);
}

void Renderer::deleteRenderer()
{
	SDL_DestroyRenderer(sdlRenderer);
}

