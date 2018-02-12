#define SDL_MAIN_HANDLED

#include <stdio.h>
#include <stdlib.h>
#include "application.h"

using namespace std;

int main()
{
	Application app = Application();
	app.setup();
	app.draw();
	app.exit();

	return 0;
}
