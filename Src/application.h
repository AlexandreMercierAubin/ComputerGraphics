#pragma once
#include "SDL2/SDL.h"
#include "Renderer.h"
#include <GL/glew.h>
#include "imgui.h"

class Application
{
	private:
		Scene::KeyFlags keyFlags;

		SDL_Window *window;
		SDL_GLContext context;
		Renderer renderer;
		void F11Keypress();
		void windowEvents(const SDL_Event &event);
		void keydownEvent(const SDL_Event &event, int &quit);
		void keyupEvent(const SDL_Event &event);

		void setupWindow(SDL_Window **window, SDL_GLContext &context);
	public:

		void setup();
		void mainLoop();
		void exit();
		Application();
	

};
