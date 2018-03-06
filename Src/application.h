#pragma once
#include "SDL2/SDL.h"
#include "Renderer.h"
#include <GL/glew.h>

class Application
{
	public:
		
		void setup();
		void mainLoop();
		void exit();
		Application();
	private:
		SDL_Window *window;
		SDL_GLContext context;
		Renderer renderer;
		void F11Keypress();

		void setupWindow(SDL_Window **window, SDL_GLContext &context);
};
