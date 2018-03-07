#pragma once
#include<vector>

#include <SDL2/SDL.h>
#include <GL/glew.h>
#include <assimp/Importer.hpp>

#include "imgui.h"
#include "imgui_internal.h"
#include "imconfig.h"
#include "imgui_impl_sdl_gl3.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "KochShader.h"
#include "ShaderLoader.h"
#include "ModelShader.h"
#include "SkyboxShader.h"
#include "Model.h"
#include "Scene.h"

class Renderer
{
public:
	void setupRenderer(SDL_Window * window, SDL_GLContext *context);
	void drawRenderer();
	void deleteRenderer();

	void screenShot(int x, int y, int w, int h, const char * filename);

	Scene scene;
	glm::mat4 perspective;
private:
	SDL_Window * window;
	SDL_GLContext gl;
	SDL_Renderer *sdlRenderer;
	GLuint shaderID;
	GLuint skyboxID;
	GLuint bufferID;
	GLuint bufferColorID;

	glm::vec3 BackgroundColor;
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

	// Options de dessin
	glm::vec4 couleurRemplissage;
	glm::vec4 couleurBordure;
	int epaisseurBordure = 0;
	bool utiliserSkybox = true;
	int formeADessiner = 0; // 0 = point, 1 = ligne, 2 = triangle, 3 = rectangle, 4 = ellipse
	int typeCurseur = 0; // 0 = par défaut, 1 = ligne, 2 = croix, 3 = triangle, 4 = rectangle, 5 = ellipse

	void drawGUI();
	void drawCursor();
	void importerImage(string file);
	void importerModele(string file);
};
