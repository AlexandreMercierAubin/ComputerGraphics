#pragma once
#include<vector>

#include <SDL2/SDL.h>
#include <GL/glew.h>
#include <assimp/Importer.hpp>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "KochShader.h"
#include "ShaderLoader.h"
#include "ModelShader.h"
#include "Model.h"
#include "Scene.h"

class Renderer
{
public:
	void setupRenderer(SDL_Window * window, SDL_GLContext *context);
	void drawRenderer();
	void deleteRenderer();

	Scene scene;
	glm::mat4 perspective;
private:
	SDL_Window * window;
	SDL_GLContext gl;
	SDL_Renderer *sdlRenderer;
	GLuint shaderID;
	GLuint bufferID;
	GLuint bufferColorID;

	std::vector<glm::vec3> BackgroundColor;
	std::vector<glm::vec3> Lines; 
	std::vector<glm::vec3> Colors;
	
	GLuint matRotation;
	GLuint matScale;
	GLuint matTranslation;
	glm::mat4 MatView(bool staticPos);

	float testScale;

	void initShaders();
	void MatScale();
	void MatRotation();
	void MatTranslation();

	//testfunc
	void courbeKoch(glm::vec3 pointDebut, glm::vec3 pointFin, int nbIterations);
	Model g_requinModel;
	GLuint kochShaderID;
	GLuint kochBufferID;
	GLuint kochBufferColorID;


	float g_angleLumiere = glm::radians(270.0f);
	float g_intensiteLumiere = 1;
};
