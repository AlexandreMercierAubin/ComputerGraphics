#pragma once
#include<vector>

#include <SDL2/SDL.h>
#include <GL/glew.h>
#include <assimp/Importer.hpp>

#include "imgui.h"
#include "imgui_internal.h"
#include "imconfig.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "KochShader.h"
#include "ShaderLoader.h"
#include "Scene.h"

class Renderer
{
public:
	void setupRenderer(SDL_Window * window, SDL_GLContext *context);
	void drawRenderer();
	void deleteRenderer();

	void screenShot(int x, int y, int w, int h, const char * filename);

	Scene scene;
private:
	SDL_Window * window;
	SDL_GLContext gl;
	SDL_Renderer *sdlRenderer;

	glm::vec3 BackgroundColor;
	std::vector<glm::vec3> Lines; 
	std::vector<glm::vec3> Colors;
	
	GLuint matRotation;
	GLuint matScale;
	GLuint matTranslation;

	float testScale;

	void initShaders();

	//for the koch test, remove that when done
	void MatScale();
	void MatRotation();
	void MatTranslation();
	

	//testfunc
	void courbeKoch(glm::vec3 pointDebut, glm::vec3 pointFin, int nbIterations);
	GLuint kochShaderID;
	GLuint kochBufferID;
	GLuint kochBufferColorID;
	// end of koch test

};
