#pragma once
#include "SDL.h"
#include "renderer.h"

class Application
{
	public:
		Renderer renderer;

		void setup();
		void draw();
		void exit();
		Application();
	private:
		SDL_Window * window;
		SDL_Surface* screenSurface;
};
