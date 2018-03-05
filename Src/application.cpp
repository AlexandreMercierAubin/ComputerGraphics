#pragma once
#include "Application.h"

//Screen dimension constants
const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;


void Application::setup()
{
	renderer = Renderer();

	//The window we'll be rendering to
	window = NULL;

	//Initialize SDL
	if (SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		SDL_Log("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
	}
	else
	{
		setupWindow(&window, context);
	}

	if (window == NULL)
	{
		SDL_Log("Window could not be created! SDL_Error: %s\n", SDL_GetError());
		SDL_Quit();
		exit();
	}
	else
	{
		renderer.setupRenderer(window, &context);
	}

}

void Application::setupWindow(SDL_Window **window, SDL_GLContext &context)
{
	//Create window
	*window = SDL_CreateWindow("Infographics", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
								SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN);

	//create the opengl context
	context = SDL_GL_CreateContext(*window);


	//set the gl version to be loaded
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

	/* Turn on double buffering with a 24bit Z buffer.
	* You may need to change this to 16 or 32 for your system */
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
	/* This makes our buffer swap syncronized with the monitor's vertical refresh */
	SDL_GL_SetSwapInterval(1);
}

void Application::draw()
{
	SDL_Event event;
	int quit = 0;

	while (!quit) {

		/* Poll for events */
		while (SDL_PollEvent(&event)) {

			switch (event.type) {
				/* Keyboard event */
				/* Pass the event data onto PrintKeyInfo() */
			case SDL_KEYDOWN:
				if (event.key.keysym.sym == SDLK_ESCAPE || event.key.keysym.sym == SDLK_q)
					SDL_Quit();
				break;

			case SDL_KEYUP:
				break;

			case SDL_QUIT:
				quit = 1;
				break;

			default:
				renderer.drawRenderer();
				break;
			}

		}

	}
}

void Application::exit()
{
	SDL_Log("<app::exit>");

	//Destroy Renderer
	renderer.deleteRenderer();

	//Destroy window
	SDL_DestroyWindow(window);

	// Delete our OpengL context
	SDL_GL_DeleteContext(context);

	//Quit SDL subsystems
	SDL_Quit();

}

Application::Application()
{
	
}
