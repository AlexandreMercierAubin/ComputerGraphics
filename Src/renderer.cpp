#pragma once
#include "Renderer.h"



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

	scene.setupScene();

	BackgroundColor = glm::vec3(0.0f, 0.0f, 0.0f);
	couleurRemplissage = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
	couleurBordure = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);

	srand(static_cast <unsigned> (time(0)));
	testScale = 0;


	// Setup ImGUI
	ImGui::CreateContext();
	ImGui::GetIO().ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;  // Enable Keyboard Controls
	ImGui_ImplSdlGL3_Init(window);
	ImGui::StyleColorsDark();
}

void Renderer::initShaders()
{
	//test, remove that 
	Core::ShaderLoader loader;
	KochShader kochShader;
	kochShaderID = loader.CreateProgram(kochShader);


	glGenBuffers(1, &kochBufferID);
	glGenBuffers(1, &kochBufferColorID);


	matRotation = glGetUniformLocation(kochShaderID, "matRotation");
	matScale = glGetUniformLocation(kochShaderID, "matScale");
	matTranslation = glGetUniformLocation(kochShaderID, "matTranslation");

	float pythagore = sqrtf(pow(0.5f, 2.f) / 2.f);
	courbeKoch(glm::vec3(-pythagore, pythagore, 0), glm::vec3(pythagore, pythagore, 0), 4);
	courbeKoch(glm::vec3(pythagore, pythagore, 0), glm::vec3(0, -0.5, 0), 4);
	courbeKoch(glm::vec3(0, -0.5, 0), glm::vec3(-pythagore, pythagore, 0), 4);

	//glUseProgram(kochShaderID);
	glUseProgram(kochShaderID);
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


void Renderer::drawRenderer()
{
	glUseProgram(kochShaderID);

	glClearColor(BackgroundColor[0], BackgroundColor[1], BackgroundColor[2], 0);// background
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	MatRotation();
	MatScale();
	MatTranslation();

	glBindBuffer(GL_ARRAY_BUFFER, kochBufferID);
	glBufferData(GL_ARRAY_BUFFER, Lines.size() * sizeof(glm::vec3), Lines.data(), GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);

	glBindBuffer(GL_ARRAY_BUFFER, kochBufferColorID);
	glBufferData(GL_ARRAY_BUFFER, Colors.size() * sizeof(glm::vec3), Colors.data(), GL_STATIC_DRAW);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, NULL);

	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);

	glLineWidth(epaisseurBordure);
	glDrawArrays(GL_LINES, 0, Lines.size());

	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);

	//glm::vec3 temp1(0.0f, -0.2f, 0.5f); glm::vec3 temp2(0.0028f, 0.0028f, 0.0028f);

	scene.drawScene();
	
	if (utiliserSkybox)
		scene.drawSkybox();

	drawGUI();
	drawCursor();

	//swap buffer
	SDL_GL_SwapWindow(window);

	testScale += 0.05f;
}



void Renderer::deleteRenderer()
{
	
	glDeleteProgram(kochShaderID);
	glDeleteBuffers(1, &kochBufferID);
	
	scene.deleteScene();
}

void Renderer::screenShot(int x, int y, int w, int h, const char * filename)
{
	unsigned char *pixels = new unsigned char[w*h * 4]; // 4 bytes for RGBA
	glReadPixels(x, y, w, h, GL_BGRA, GL_UNSIGNED_BYTE, pixels);

	SDL_Surface * surf = SDL_CreateRGBSurfaceFrom(pixels, w, h, 8 * 4, w * 4, 0, 0, 0, 0);
	SDL_SaveBMP(surf, filename);

	SDL_FreeSurface(surf);
	delete[] pixels;
}

void Renderer::drawGUI()
{
	ImGui_ImplSdlGL3_NewFrame(window);

	// ********** Importer **********

	ImGui::Begin("Importer");

	static char fichier[1000] = "";
	ImGui::InputText("Fichier", fichier, IM_ARRAYSIZE(fichier));

	if (ImGui::Button("Importer image"))
		importerImage(string(fichier));

	ImGui::SameLine();
	if (ImGui::Button("Importer modele 3D"))
		importerModele(string(fichier));

	ImGui::End();

	// ********** Options de dessin **********

	ImGui::Begin("Options de dessin");

	ImGui::ColorEdit4("Remplissage", &couleurRemplissage.r);
	ImGui::ColorEdit4("Bordures", &couleurBordure.r);
	ImGui::SliderInt("Epaisseur bordures", &epaisseurBordure, 1, 10);
	ImGui::Combo("Forme a dessiner", &formeADessiner, "Point\0Ligne\0Triangle\0Rectangle\0Ellipse");

	ImGui::NewLine();

	ImGui::Checkbox("Utiliser skybox", &utiliserSkybox);
	ImGui::ColorEdit3("Arriere-plan", &BackgroundColor.r);

	ImGui::NewLine();

	ImGui::Combo("Curseur", &typeCurseur, "Defaut\0Ligne\0Croix\0Triangle\0Rectangle\0Ellipse");

	ImGui::End();

	// Render
	ImGui::Render();
	ImGui_ImplSdlGL3_RenderDrawData(ImGui::GetDrawData());
}

void Renderer::drawCursor()
{
	if (typeCurseur == 0)
		SDL_ShowCursor(SDL_ENABLE);
	else
	{
		SDL_ShowCursor(SDL_DISABLE);

		int x = 0;
		int y = 0;
		SDL_GetMouseState(&x, &y);

		switch (typeCurseur)
		{
		case 1: // Ligne
			break;

		case 2: // Croix
			break;

		case 3: // Triangle
			break;

		case 4: // Rectangle
			break;

		case 5: // Ellipse
			break;
		}
	}
}

void Renderer::importerImage(string fichier)
{
	// TO-DO
}

void Renderer::importerModele(string fichier)
{
	// TO-DO
}

