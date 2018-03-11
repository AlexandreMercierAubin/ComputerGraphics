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

#include "ShaderLoader.h"
#include "Scene.h"

#include "PrimitiveObject.h"
#include "SphereObject.h"
#include "SBPyramidObject.h"
#include "PrimitiveShader.h"
#include "TexShader.h"
#include "SimpleGPShader.h"


class Renderer
{
public:
	void setupRenderer(SDL_Window * window, SDL_GLContext *context);
	void drawRenderer(Scene::KeyFlags &flags);
	void resize(const int &w,const int &h);
	void mouseMotion(const unsigned int &timestamp,const unsigned int &windowID, const unsigned int &state, const int &x, const int &y, const int &xRel, const int &yRel,Scene::KeyFlags flags);

	void screenShot(int x, int y, int w, int h, const char * filename);

	void ajouterPtDessin(int x, int y);
	void eraseNodes();

	Scene scene;
	~Renderer();

private:
	SDL_Window * window;
	SDL_GLContext gl;
	SDL_Renderer *sdlRenderer;

	glm::vec3 BackgroundColor;
	std::vector<glm::vec3> Lines; 
	std::vector<glm::vec3> Colors;

	float testScale;

	void initShaders();

	// Options de dessin
	glm::vec4 couleurRemplissage;
	glm::vec4 couleurBordure;
	int epaisseurBordure = 1;
	bool utiliserSkybox = true;
	int formeADessiner = 0; // 0 = point, 1 = ligne, 2 = triangle, 3 = rectangle, 4 = quad, 5 = smiley, 6 = étoile , 7 = Cube, 8 = Sphere
	std::vector<glm::vec3> ptsDessin;

	int typeCurseur = 0; // 0 = par défaut, 1 = point, 2 = points, 3 = croix, 4 = triangle, 5 = quad
	PrimitiveObject curseur;

	GLuint primitiveShaderID;
	GLuint GPShaderID;
	GLuint modelShaderID;
	GLuint texShaderID;

	// Échantillonnage d’image
	int postionEchantillonnage = 0; // 0 = Haut-Gauche, 1 = Haut-Milieu, 2 = Milieu-Gauche, 3 = Milieu-Milieu
	int pourcentageImage = 0;

	// Transformations
	glm::vec4 currentColor;
	glm::vec3 currentTranslation;
	glm::vec3 currentRotation;
	glm::quat currentRotationQuat;
	glm::vec3 currentScale;
	bool proportionalResizing = true;
	bool useQuaternion = false;
	void setColor();
	void addTranslation(const glm::vec3 &v);
	void addRotation(const glm::vec3 &v);
	void addRotation(const glm::quat &q);
	void addScale(const glm::vec3 &v);
	void updateTransformations();

	std::vector<std::pair<std::shared_ptr<AbstractObject>, std::shared_ptr<GroupObject>>> selectedNodes;
	std::shared_ptr<GroupObject> castToGroupObject(std::shared_ptr<AbstractObject> obj);
	void deselectAllNodes();
	void deselectNode(std::shared_ptr<AbstractObject> obj);
	void selectNode(std::shared_ptr<AbstractObject> obj, std::shared_ptr<GroupObject> parent);
	void groupNodes();

	void drawGUI();
	void drawTreeRecursive(std::shared_ptr<GroupObject> objects);
	void drawCursor();
	void updateCursor();
	void importImage(string file);
	void importModel(string file);
	void imagePerlinNoise(string file);
	void imageComposition(string file);
	void echantillonnageImage(string file,string file2);

	void ajouterSmiley();
	void ajouterEtoile();
	void addCube();
	void addSBPyramid();
};
