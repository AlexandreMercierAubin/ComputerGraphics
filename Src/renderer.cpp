#include "renderer.h"

void Renderer::setupRenderer(SDL_Window * window, SDL_GLContext *context)
{
	this->window = window;
}

void Renderer::drawRenderer()
{
	GLuint testLib;

	glClearColor(0.0, 255.0, 0.0, 1.0);
	glClear(GL_COLOR_BUFFER_BIT);

	//oldGlTest
	glLineWidth(2.5);
	glColor3f(1.0, 0.0, 0.0);
	glBegin(GL_LINES);
	glVertex3f(0.0, 0.0, 0.0);
	glVertex3f(15, 0, 0);
	glEnd();

	//swap buffer
	SDL_GL_SwapWindow(window);

	//Wait two seconds
	SDL_Delay(50000);
}

void Renderer::deleteRenderer()
{
	SDL_DestroyRenderer(sdlRenderer);
}

