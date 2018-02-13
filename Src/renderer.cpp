#include "renderer.h"


void Renderer::setupRenderer(SDL_Window * window, SDL_GLContext *context)
{
	this->window = window;

	#ifndef __APPLE__
		glewExperimental = GL_TRUE;
		glewInit();
	#endif

	initShaders();
	glUseProgram(shaderID);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	BackgroundColor.push_back(glm::vec3(0.0, 1.0, 1.0));
	srand(static_cast <unsigned> (time(0)));
	testScale = 0;


}

void Renderer::initShaders()
{
	Core::Shader_Loader loader;
	shaderID = loader.CreateProgram("vertexShaderKoch.glsl", "fragmentShaderKoch.glsl");

	glGenBuffers(1, &bufferID);
	glGenBuffers(1, &bufferColorID);

	matRotation = glGetUniformLocation(shaderID, "matRotation");
	matScale = glGetUniformLocation(shaderID, "matScale");
	matTranslation = glGetUniformLocation(shaderID, "matTranslation");

	float pythagore = sqrtf(pow(0.5f, 2.f) / 2.f);
	courbeKoch(glm::vec3(-pythagore, pythagore, 0), glm::vec3(pythagore, pythagore, 0), 4);
	courbeKoch(glm::vec3(pythagore, pythagore, 0), glm::vec3(0, -0.5, 0), 4);
	courbeKoch(glm::vec3(0, -0.5, 0), glm::vec3(-pythagore, pythagore, 0), 4);
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

void Renderer::MatTranslation(int index) // matrice de translation
{
	glm::mat4 trans;
	if (index == 0)
	{
		trans = glm::mat4(1.0);
	}
	else if (index == 1)
	{
		trans[0][0] = 1.0f; trans[0][1] = 0.0f; trans[0][2] = 0.0f; trans[0][3] = 0.7f;
		trans[1][0] = 0.0f; trans[1][1] = 1.0f; trans[1][2] = 0.0f; trans[1][3] = 0.0f;
		trans[2][0] = 0.0f; trans[2][1] = 0.0f; trans[2][2] = 1.0f; trans[2][3] = 0.0f;
		trans[3][0] = 0.0f; trans[3][1] = 0.0f; trans[3][2] = 0.0f; trans[3][3] = 1.0f;
	}
	else
	{
		trans[0][0] = 1.0f; trans[0][1] = 0.0f; trans[0][2] = 0.0f; trans[0][3] = -0.7f;
		trans[1][0] = 0.0f; trans[1][1] = 1.0f; trans[1][2] = 0.0f; trans[1][3] = 0.0f;
		trans[2][0] = 0.0f; trans[2][1] = 0.0f; trans[2][2] = 1.0f; trans[2][3] = 0.0f;
		trans[3][0] = 0.0f; trans[3][1] = 0.0f; trans[3][2] = 0.0f; trans[3][3] = 1.0f;
	}


	glUniformMatrix4fv(matTranslation, 1, GL_TRUE, &trans[0][0]);
}


void Renderer::drawRenderer()
{
	glClearColor(0.0, 255.0, 0.0, 1.0);
	glClear(GL_COLOR_BUFFER_BIT);

	//temp drawloop
	for (int i = 0; i < 100; ++i) 
	{
		glClearColor(0.0, 255.0, 0.0, 1.0);
		glClear(GL_COLOR_BUFFER_BIT);

		MatRotation();
		MatScale();
		MatTranslation(0);

		glBindBuffer(GL_ARRAY_BUFFER, bufferID);
		glBufferData(GL_ARRAY_BUFFER, Lines.size() * sizeof(glm::vec3), Lines.data(), GL_STATIC_DRAW);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);

		glBindBuffer(GL_ARRAY_BUFFER, bufferColorID);
		glBufferData(GL_ARRAY_BUFFER, Colors.size() * sizeof(glm::vec3), Colors.data(), GL_STATIC_DRAW);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, NULL);

		glEnableVertexAttribArray(0);
		glEnableVertexAttribArray(1);

		glLineWidth(2);
		glDrawArrays(GL_LINES, 0, Lines.size());

		glDisableVertexAttribArray(0);
		glDisableVertexAttribArray(1);
		//swap buffer
		SDL_GL_SwapWindow(window);

		testScale += 0.01f;
		//Wait two seconds
		SDL_Delay(10);
	}
}



void Renderer::deleteRenderer()
{
	glDeleteProgram(shaderID);
	glDeleteBuffers(1, &bufferID);
}

