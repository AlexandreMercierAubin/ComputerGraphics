#pragma once
#include "SDL2/SDL.h"
#include "Renderer.h"
#include <GL/glew.h>
#include "imgui.h"

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
		void windowEvents(const SDL_Event * event);

		void setupWindow(SDL_Window **window, SDL_GLContext &context);
};
