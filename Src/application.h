#pragma once
#include "SDL2/SDL.h"
#include "renderer.h"
#include <GL/glew.h>

class Application
{
	public:
		
		void setup();
		void draw();
		void exit();
		Application();
	private:
		SDL_Window *window;
		SDL_GLContext context;
		Renderer renderer;

		void setupWindow(SDL_Window **window, SDL_GLContext &context);
};
