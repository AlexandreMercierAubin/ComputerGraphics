#pragma once
#include "Renderer.h"

glm::vec3 g_direction(0.0, 0.0, 1.0);
glm::vec4 g_uniformCouleur(1.0, 1.0, 1.0, 1.0);

glm::vec3 g_position(0.0, 0.0, 1.0);
glm::vec3 g_orientation(0.0, 1.0, 0.0);
GLfloat g_yaw = -90;
GLfloat g_pitch = 0;

void Renderer::setupRenderer(SDL_Window * window, SDL_GLContext *context)
{
	this->window = window;

	#ifndef __APPLE__
		glewExperimental = GL_TRUE;
		glewInit();
	#endif

	initShaders();

	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glDepthFunc(GL_LEQUAL);

	BackgroundColor.push_back(glm::vec3(0.0, 1.0, 1.0));
	srand(static_cast <unsigned> (time(0)));
	testScale = 0;

	g_requinModel = Model("Resources/megalodon/megalodon.FBX",4);
}

void Renderer::initShaders()
{
	Core::ShaderLoader loader;
	ModelShader modelShader;
	KochShader kochShader;
	kochShaderID = loader.CreateProgram(kochShader);
	shaderID = loader.CreateProgram(modelShader);

	glGenBuffers(1, &kochBufferID);
	glGenBuffers(1, &kochBufferColorID);


	//matRotation = glGetUniformLocation(kochShaderID, "matRotation");
	//matScale = glGetUniformLocation(kochShaderID, "matScale");
	//matTranslation = glGetUniformLocation(kochShaderID, "matTranslation");

	float pythagore = sqrtf(pow(0.5f, 2.f) / 2.f);
	courbeKoch(glm::vec3(-pythagore, pythagore, 0), glm::vec3(pythagore, pythagore, 0), 4);
	courbeKoch(glm::vec3(pythagore, pythagore, 0), glm::vec3(0, -0.5, 0), 4);
	courbeKoch(glm::vec3(0, -0.5, 0), glm::vec3(-pythagore, pythagore, 0), 4);

	perspective = glm::perspective(glm::radians(45.0f), 800.0f / 600.0f, 0.1f, 100.0f);
	//glUseProgram(kochShaderID);
	glUseProgram(shaderID);
}

void Renderer::courbeKoch(glm::vec3 pointDebut, glm::vec3 pointFin, int nbIterations)
{
	//fonction qui permet de conserver les points d'une ligne du flocon de Koch
	std::vector<glm::vec3> ligneTemp;

	//si le nombre d'itérations est expiré
	if (nbIterations == 0)
	{
		// permets de conserver la ligne et de trouver une couleur aléatoire pour cette ligne

		//couleur aléatoire
		float x = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
		float y = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
		float z = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);

		//conservation de la couleur
		Colors.push_back(glm::vec3(x, y, z));
		Colors.push_back(glm::vec3(x, y, z));

		//conservation de la position
		Lines.push_back(pointDebut);
		Lines.push_back(pointFin);
	}
	else
	{
		//Trouver les 5 points pour dessiner les 4 lignes
		ligneTemp.push_back(pointDebut);
		ligneTemp.push_back(glm::vec3(
			(2.0 * pointDebut.x + pointFin.x) / 3.0,
			(2.0 * pointDebut.y + pointFin.y) / 3.0,
			0));//pointB

		ligneTemp.push_back(glm::vec3(
			(pointDebut.x + pointFin.x) / 2.0 - sqrt(3.0) / 6.0 *(pointFin.y - pointDebut.y),
			(pointDebut.y + pointFin.y) / 2.0 + sqrt(3.0) / 6.0 *(pointFin.x - pointDebut.x),
			0));//pointC

		ligneTemp.push_back(glm::vec3(
			(pointDebut.x + 2.0 * pointFin.x) / 3.0,
			(pointDebut.y + 2.0 * pointFin.y) / 3.0,
			0));//pointD

		ligneTemp.push_back(pointFin);

		//Itérer sur les 4 nouvelles lignes trouvées
		courbeKoch(ligneTemp[0], ligneTemp[1], nbIterations - 1);
		courbeKoch(ligneTemp[1], ligneTemp[2], nbIterations - 1);
		courbeKoch(ligneTemp[2], ligneTemp[3], nbIterations - 1);
		courbeKoch(ligneTemp[3], ligneTemp[4], nbIterations - 1);
	}
}

void Renderer::MatScale() // matrice d'échelle
{
	glm::mat4 eche;

	eche[0][0] = sinf(testScale)*1.0f; eche[0][1] = 0.0f; eche[0][2] = 0.0f; eche[0][3] = 0.0f;
	eche[1][0] = 0.0f; eche[1][1] = sinf(testScale)*1.0f; eche[1][2] = 0.0f; eche[1][3] = 0.0f;
	eche[2][0] = 0.0f; eche[2][1] = 0.0f; eche[2][2] = 1.0f; eche[2][3] = 0.0f;
	eche[3][0] = 0.0f; eche[3][1] = 0.0f; eche[3][2] = 0.0f; eche[3][3] = 1.0f;


	glUniformMatrix4fv(matScale, 1, GL_TRUE, &eche[0][0]);
}

void Renderer::MatRotation() // matrice de rotation
{
	glm::mat4 rotat;

	rotat[0][0] = cosf(testScale); rotat[0][1] = -sinf(testScale); rotat[0][2] = 0.0f; rotat[0][3] = 0.0f;
	rotat[1][0] = sinf(testScale); rotat[1][1] = cosf(testScale); rotat[1][2] = 0.0f; rotat[1][3] = 0.0f;
	rotat[2][0] = 0.0f; rotat[2][1] = 0.0f; rotat[2][2] = 1.0f; rotat[2][3] = 0.0f;
	rotat[3][0] = 0.0f; rotat[3][1] = 0.0f; rotat[3][2] = 0.0f; rotat[3][3] = 1.0f;


	glUniformMatrix4fv(matRotation, 1, GL_TRUE, &rotat[0][0]);
}

void Renderer::MatTranslation() // matrice de translation
{
	glm::mat4 trans;
	trans = glm::mat4(1.0);



	glUniformMatrix4fv(matTranslation, 1, GL_TRUE, &trans[0][0]);
}

glm::mat4 Renderer::MatView(bool staticPos) 
{
	glm::mat4 vue;
	glm::vec3 front;
	glm::vec3 position;

	g_position.y = 0;

	front.x = cos(glm::radians(g_yaw)) * cos(glm::radians(g_pitch));
	front.y = sin(glm::radians(g_pitch));
	front.z = sin(glm::radians(g_yaw)) * cos(glm::radians(g_pitch));

	g_direction = glm::normalize(front);

	if (staticPos)
	{
		position = glm::vec3(0.0, 0.0, 1.0);
	}
	else
	{
		position = g_position;
	}

	vue = glm::lookAt(position, g_direction + position, g_orientation);
	return vue;
}

void Renderer::drawRenderer()
{
	glUseProgram(kochShaderID);

	glClearColor(0, 1.0, 1.0, 0);// background
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//MatRotation();
	//MatScale();
	//MatTranslation();

	//glBindBuffer(GL_ARRAY_BUFFER, kochBufferID);
	//glBufferData(GL_ARRAY_BUFFER, Lines.size() * sizeof(glm::vec3), Lines.data(), GL_STATIC_DRAW);
	//glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);

	//glBindBuffer(GL_ARRAY_BUFFER, kochBufferColorID);
	//glBufferData(GL_ARRAY_BUFFER, Colors.size() * sizeof(glm::vec3), Colors.data(), GL_STATIC_DRAW);
	//glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, NULL);

	//glEnableVertexAttribArray(0);
	//glEnableVertexAttribArray(1);

	//glLineWidth(2);
	//glDrawArrays(GL_LINES, 0, Lines.size());

	//glDisableVertexAttribArray(0);
	//glDisableVertexAttribArray(1);

	glm::vec3 temp1(0.0f, -0.2f, 0.5f); glm::vec3 temp2(0.0028f, 0.0028f, 0.0028f);

	glUseProgram(shaderID);
	glm::mat4 view = MatView(false);
	scene.drawModel(shaderID, view, perspective ,g_requinModel, temp1, temp2, g_uniformCouleur, g_intensiteLumiere, g_direction);

	//swap buffer
	SDL_GL_SwapWindow(window);

	testScale += 0.05f;
	//Wait two seconds
	//SDL_Delay(50);
	
}



void Renderer::deleteRenderer()
{
	glDeleteProgram(shaderID);
	glDeleteProgram(kochShaderID);
	glDeleteBuffers(1, &kochBufferID);
	glDeleteBuffers(1, &bufferID);
	scene.deleteScene();
}

void Renderer::screenShot(int x, int y, int w, int h, const char * filename)
{
	unsigned char * pixels = new unsigned char[w*h * 4]; // 4 bytes for RGBA
	glReadPixels(x, y, w, h, GL_RGBA, GL_UNSIGNED_BYTE, pixels);

	SDL_Surface * surf = SDL_CreateRGBSurfaceFrom(pixels, w, h, 8 * 4, w * 4, 0, 0, 0, 0);
	SDL_SaveBMP(surf, filename);

	SDL_FreeSurface(surf);
	delete[] pixels;
}

