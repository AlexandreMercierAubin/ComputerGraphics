#pragma once
#include<vector>

#include <SDL2/SDL.h>
#include <GL/glew.h>
#include <assimp/Importer.hpp>
#include <stack>

#include "imgui.h"
#include "imgui_internal.h"
#include "imconfig.h"
#include "imgui_impl_sdl_gl3.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "KochShader.h"
#include "ShaderLoader.h"
#include "Scene.h"

#include "PrimitiveObject.h"
#include "PrimitiveShader.h"
#include "SimpleTexShader.h"

class Renderer
{
public:
	void setupRenderer(SDL_Window * window, SDL_GLContext *context);
	void drawRenderer(Scene::KeyFlags &flags);
	void resize(const int &w,const int &h);
	void mouseMotion(const unsigned int &timestamp,const unsigned int &windowID, const unsigned int &state, const int &x, const int &y, const int &xRel, const int &yRel,Scene::KeyFlags flags);

	void screenShot(int x, int y, int w, int h, const char * filename);
	void ajouterPtDessin(int x, int y);

	Scene scene;
	~Renderer();

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


	// Options de dessin
	glm::vec4 couleurRemplissage;
	glm::vec4 couleurBordure;
	int epaisseurBordure = 1;
	bool utiliserSkybox = true;
	int formeADessiner = 0; // 0 = point, 1 = ligne, 2 = triangle, 3 = rectangle, 4 = quad, 5 = smiley, 6 = �toile
	std::vector<glm::vec3> ptsDessin;

	int typeCurseur = 0; // 0 = par d�faut, 1 = point, 2 = points, 3 = croix, 4 = triangle, 5 = quad
	PrimitiveObject curseur;

	GLuint primitiveShaderID;
	GLuint simpleTexShaderID;
	GLuint modelShaderID;

	std::vector<std::tuple<std::shared_ptr<AbstractObject>, std::shared_ptr<GroupObject>, int>> selectedNodes;
	void deselectAllNodes();
	std::shared_ptr<GroupObject> castToGroupObject(std::shared_ptr<AbstractObject> obj);

	void drawGUI();
	void drawTreeRecursive(std::shared_ptr<GroupObject> objects);
	void drawCursor();
	void updateCursor();
	void importerImage(string file);
	void importModel(string file);

	void ajouterSmiley();
	void ajouterEtoile();
};
