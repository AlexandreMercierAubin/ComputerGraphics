#include "application.h"

//Screen dimension constants
const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;


void Application::setup()
{
	renderer = Renderer();

	//The window we'll be rendering to
	window = NULL;

	//The surface contained by the window
	screenSurface = NULL;

	//Initialize SDL
	if (SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		SDL_Log("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
	}
	else
	{
		//Create window
		window = SDL_CreateWindow("Infographics", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
		if (window == NULL)
		{
			SDL_Log("Window could not be created! SDL_Error: %s\n", SDL_GetError());
		}
		else
		{
			//Get window surface
			screenSurface = SDL_GetWindowSurface(window);

			renderer.setup(window, screenSurface);
		}
	}

	

}

void Application::draw()
{
	renderer.draw();
}

void Application::exit()
{
	SDL_Log("<app::exit>");

	//Destroy renderer
	renderer.exit();

	//Release surface
	SDL_FreeSurface(screenSurface);

	//Destroy window
	SDL_DestroyWindow(window);

	//Quit SDL subsystems
	SDL_Quit();

}

Application::Application()
{
	
}
