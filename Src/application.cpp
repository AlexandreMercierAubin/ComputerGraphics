#pragma once
#include "Application.h"


void Application::setup()
{
	keyFlags.flagDown = false;
	keyFlags.flagUp = false;
	keyFlags.flagRight = false;
	keyFlags.flagLeft = false;
	keyFlags.flagLeftMouse = false;
	keyFlags.flagRightMouse = false;

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
	int initialWidth = 1200;
	int initialHeigth = 800;
	*window = SDL_CreateWindow("Infographics", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
								initialWidth, initialHeigth, SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN);

	// Make window resizable
	SDL_SetWindowResizable(*window, SDL_TRUE);

	//create the opengl context
	context = SDL_GL_CreateContext(*window);


	//set the gl version to be loaded
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 2);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

	/* Turn on double buffering with a 24bit Z buffer.
	* You may need to change this to 16 or 32 for your system */
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
	/* This makes our buffer swap syncronized with the monitor's vertical refresh */
	SDL_GL_SetSwapInterval(1);
}

void Application::F11Keypress() 
{
	int width;
	int height;

	SDL_GetWindowSize(window, &width, &height);
	renderer.screenShot(0, 0, width, height, "Screenshot.bmp");

}

void Application::mainLoop()
{
	SDL_Event event;
	int quit = 0;

	while (!quit) {

		/* Poll for events */
		while (SDL_PollEvent(&event)) {

			// ImGui procces event pour écrire dans textbox, etc.
			ImGui_ImplSdlGL3_ProcessEvent(&event);

			switch (event.type) {
				/* Keyboard event */
				/* Pass the event data onto PrintKeyInfo() */
			case SDL_MOUSEBUTTONDOWN:
				if (event.button.button == SDL_BUTTON_LEFT)
					keyFlags.flagLeftMouse = true;
				else if (event.button.button = SDL_BUTTON_RIGHT)
					keyFlags.flagRightMouse = true;
				break;

			case SDL_MOUSEBUTTONUP:
				if (event.button.button == SDL_BUTTON_LEFT)
				{
					keyFlags.flagLeftMouse = false;

					if (!ImGui::GetIO().WantCaptureMouse)
						renderer.ajouterPtDessin(event.button.x, event.button.y);
				}
				else if (event.button.button == SDL_BUTTON_RIGHT)
					keyFlags.flagRightMouse = false;
				break;

			case SDL_KEYDOWN:
				keydownEvent(event,quit);
				break;

			case SDL_WINDOWEVENT:
				windowEvents(event);
				break;

			case SDL_KEYUP:
				keyupEvent(event);
				break;

			case SDL_MOUSEMOTION:
				renderer.mouseMotion(event.motion.timestamp, event.motion.windowID, event.motion.state, event.motion.x, event.motion.y, event.motion.xrel, event.motion.yrel, keyFlags);
				break;

			case SDL_QUIT:
				quit = 1;
				break;
			}
		}

		renderer.drawRenderer(keyFlags);
	}
}

void Application::keydownEvent(const SDL_Event &event, int &quit)
{
	switch (event.key.keysym.sym) 
	{
	case SDLK_ESCAPE:
		quit = 1;
		break;
	case SDLK_F11:
		F11Keypress();
		break;
	case SDLK_UP:
		keyFlags.flagUp = true;
		break;
	case SDLK_DOWN:
		keyFlags.flagDown = true;
		break;
	case SDLK_LEFT:
		keyFlags.flagLeft = true;
		break;
	case SDLK_RIGHT:
		keyFlags.flagRight = true;
		break;
	}
}

void Application::keyupEvent(const SDL_Event &event)
{
	switch (event.key.keysym.sym)
	{
	case SDLK_UP:
		keyFlags.flagUp = false;
		break;
	case SDLK_DOWN:
		keyFlags.flagDown = false;
		break;
	case SDLK_LEFT:
		keyFlags.flagLeft = false;
		break;
	case SDLK_RIGHT:
		keyFlags.flagRight = false;
		break;
	}
}

void Application::windowEvents(const SDL_Event &event)
{
	switch (event.window.event)
	{
	case SDL_WINDOWEVENT_SIZE_CHANGED:
		renderer.resize(event.window.data1, event.window.data2);
		break;

	case SDL_WINDOWEVENT_RESIZED:
		renderer.resize(event.window.data1, event.window.data2);
		break;
	}
}

void Application::exit()
{
	SDL_Log("<app::exit>");

	// Exit ImGui
	ImGui_ImplSdlGL3_Shutdown();
	ImGui::DestroyContext();

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
