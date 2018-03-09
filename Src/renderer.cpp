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
	
	// Do not remove
	PrimitiveShader primitiveShader;
	primitiveShaderID = loader.CreateProgram(primitiveShader);
	SimpleTexShader simpleTexShader;
	simpleTexShaderID = loader.CreateProgram(simpleTexShader);
	ModelShader modelShader;
	modelShaderID = loader.CreateProgram(modelShader);

	curseur.Create(primitiveShaderID);
	curseur.setCouleurRemplissage(glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));
	curseur.setCouleurBordure(glm::vec4(0.0f, 0.0f, 0.0f, 1.0f));
	// End do not remove


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

	//si le nombre d'it�rations est expir�
	if (nbIterations == 0)
	{
		// permets de conserver la ligne et de trouver une couleur al�atoire pour cette ligne

		//couleur al�atoire
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

		//It�rer sur les 4 nouvelles lignes trouv�es
		courbeKoch(ligneTemp[0], ligneTemp[1], nbIterations - 1);
		courbeKoch(ligneTemp[1], ligneTemp[2], nbIterations - 1);
		courbeKoch(ligneTemp[2], ligneTemp[3], nbIterations - 1);
		courbeKoch(ligneTemp[3], ligneTemp[4], nbIterations - 1);
	}
}

void Renderer::MatScale() // matrice d'�chelle
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


void Renderer::drawRenderer(Scene::KeyFlags &flags)
{
	/*glUseProgram(kochShaderID);*/

	glClearColor(BackgroundColor[0], BackgroundColor[1], BackgroundColor[2], 0);// background
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

	//glLineWidth((GLfloat)epaisseurBordure);
	//glDrawArrays(GL_LINES, 0, Lines.size());

	//glDisableVertexAttribArray(0);
	//glDisableVertexAttribArray(1);

	//glm::vec3 temp1(0.0f, -0.2f, 0.5f); glm::vec3 temp2(0.0028f, 0.0028f, 0.0028f);
	scene.refreshScene(flags);

	if (utiliserSkybox)
		scene.drawSkybox();

	scene.drawScene();
	


	drawGUI();

	// Affiche le curseur s�lectionn�
	if (typeCurseur == 0)
		SDL_ShowCursor(SDL_ENABLE);
	else
	{
		SDL_ShowCursor(SDL_DISABLE);
		drawCursor();
	}

	//swap buffer
	SDL_GL_SwapWindow(window);

	testScale += 0.05f;
}


void Renderer::resize(const int & w, const int & h)
{
	glViewport(0, 0, w, h);
}

void Renderer::mouseMotion(const unsigned int & timestamp, const unsigned int & windowID, const unsigned int & state, const int & x, const int & y, const int & xRel, const int & yRel ,Scene::KeyFlags flags)
{
	if (SDL_GetWindowID(window)==windowID && (SDL_GetMouseState(nullptr, nullptr) & SDL_BUTTON(SDL_BUTTON_RIGHT))) 
	{
		scene.mouseMotion(timestamp, windowID, state, x, y, xRel, yRel);
		drawRenderer(flags);
	}
}

void Renderer::screenShot(int x, int y, int w, int h, const char * filename)
{
	unsigned int size = w * h * 4;
	unsigned char *pixels = new unsigned char[size]; // 4 bytes for RGBA
	glReadBuffer(GL_FRONT);
	glReadPixels(x, y, w, h, GL_BGRA, GL_UNSIGNED_BYTE, pixels);

	//vertical flip cause glRead goes backwards for some reason
	unsigned char *flipPixels=new unsigned char[size];
	for (int i = 0; i < w; ++i) {
		for (int j = 0; j < h; ++j) {
			for (int k = 0; k < 4; ++k) {
				flipPixels[(i + j * w) * 4 + k] = pixels[(i + (h - 1 - j) * w) * 4 + k];
			}
		}
	}

	SDL_Surface * surf = SDL_CreateRGBSurfaceFrom(flipPixels, w, h, 8 * 4, w * 4, 0, 0, 0, 0);
	SDL_SaveBMP(surf, filename);

	SDL_FreeSurface(surf);
	delete[] pixels;
	delete[] flipPixels;
	
}


Renderer::~Renderer()
{
	glDeleteProgram(kochShaderID);
	glDeleteBuffers(1, &kochBufferID);
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

	if (ImGui::Button("Importer modele 3D"))
		importModel(string(fichier));

	if (ImGui::Button("Capture d'ecran"))
	{
		string fileName = fichier;
		if (fileName == "")
			fileName = "Screenshot.bmp";
		else if (fileName.substr(fileName.length() - 4) != ".bmp")
			fileName += ".bmp";

		int w, h;
		SDL_GetWindowSize(window, &w, &h);
		screenShot(0, 0, w, h, fileName.c_str());
	}

	ImGui::End();

	// ********** Options de dessin **********

	ImGui::Begin("Options de dessin");

	ImGui::ColorEdit4("Remplissage", &couleurRemplissage.r);
	ImGui::ColorEdit4("Bordures", &couleurBordure.r);
	ImGui::SliderInt("Epaisseur bordures", &epaisseurBordure, 0, 10);
	if (ImGui::Combo("Forme a dessiner", &formeADessiner, "Point\0Ligne\0Triangle\0Rectangle\0Quad\0Smiley\0Etoile\0"))
		ptsDessin.clear();

	ImGui::NewLine();

	ImGui::Checkbox("Utiliser skybox", &utiliserSkybox);
	ImGui::ColorEdit3("Arriere-plan", &BackgroundColor.r);

	ImGui::NewLine();

	if (ImGui::Combo("Curseur", &typeCurseur, "Defaut\0Point\0Points\0Croix\0Triangle\0Quad\0"))
		updateCursor();

	ImGui::End();

	// ********** Graphe de sc�ne **********

	ImGui::Begin("Graphe de scene");

	if (ImGui::Button("Grouper"))
		groupNodes();
	
	ImGui::SameLine();
	if (ImGui::Button("Effacer"))
		eraseNodes();
	
	std::shared_ptr<GroupObject> root = scene.getObjects();
	bool nodeSelected = false;

	ImGuiTreeNodeFlags nodeFlags = ImGuiTreeNodeFlags_OpenOnArrow;
	if (root->isSelected())
	{
		nodeFlags |= ImGuiTreeNodeFlags_Selected;
		nodeSelected = true;
	}
	
	bool nodeOpen = ImGui::TreeNodeEx("Racine", nodeFlags);
	if (ImGui::IsItemClicked())
	{
		if (!ImGui::GetIO().KeyCtrl)
			deselectAllNodes();

		root->setSelected(!nodeSelected);
		selectedNodes.push_back(std::make_pair(root, nullptr));
	}
	if (nodeOpen)
	{
		drawTreeRecursive(root);
		ImGui::TreePop();
	}

	ImGui::End();

	// Render
	ImGui::Render();
	ImGui_ImplSdlGL3_RenderDrawData(ImGui::GetDrawData());
}

void Renderer::drawTreeRecursive(std::shared_ptr<GroupObject> objects)
{
	for (unsigned int i = 0; i < objects->size(); ++i)
	{
		std::shared_ptr<AbstractObject> obj = objects->getObjectAt(i);
		std::shared_ptr<GroupObject> group = castToGroupObject(obj);

		bool isGroup = group != nullptr;
		bool nodeSelected = false;

		ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_OpenOnArrow;
		if (!isGroup)
			flags |= ImGuiTreeNodeFlags_Leaf | ImGuiTreeNodeFlags_NoTreePushOnOpen;

		if (obj->isSelected())
		{
			flags |= ImGuiTreeNodeFlags_Selected;
			nodeSelected = true;
		}

		bool nodeOpen = ImGui::TreeNodeEx(obj.get(), flags, obj->getName().c_str());
		if (ImGui::IsItemClicked())
		{
			if (!ImGui::GetIO().KeyCtrl)
				deselectAllNodes();

			obj->setSelected(!nodeSelected);
			selectedNodes.push_back(std::make_pair(obj, objects));
		}
		if (nodeOpen && isGroup)
		{
			drawTreeRecursive(group);
			ImGui::TreePop();
		}
	}
}

void Renderer::drawCursor()
{
	int x = 0;
	int y = 0;
	int w = 0;
	int h = 0;
	SDL_GetMouseState(&x, &y);
	SDL_GetWindowSize(window, &w, &h);

	// Conversion en coordonn�es centr�es sur la fen�tre
	float halfW = w / 2.0f;
	float halfH = h / 2.0f;
	float glX = x / halfW - 1.0f;
	float glY = -y / halfH + 1.0f;

	std::vector<glm::vec3> positions;

	switch (typeCurseur)
	{
	case 1: // Point
		positions.push_back(glm::vec3(glX, glY, 0.0f));
		break;
		
	case 2: // Points
		positions.push_back(glm::vec3(glX, glY, 0.0f));
		positions.push_back(glm::vec3(glX, glY + 10.0f / halfH, 0.0f));
		positions.push_back(glm::vec3(glX, glY - 10.0f / halfH, 0.0f));
		positions.push_back(glm::vec3(glX - 10.0f / halfW, glY, 0.0f));
		positions.push_back(glm::vec3(glX + 10.0f / halfW, glY, 0.0f));
		break;

	case 3: // Croix
		positions.push_back(glm::vec3(glX, glY + 10.0f / halfH, 0.0f));
		positions.push_back(glm::vec3(glX, glY - 10.0f / halfH, 0.0f));
		positions.push_back(glm::vec3(glX - 10.0f / halfW, glY, 0.0f));
		positions.push_back(glm::vec3(glX + 10.0f / halfW, glY, 0.0f));
		break;

	case 4: // Triangle
		positions.push_back(glm::vec3(glX, glY, 0.0f));
		positions.push_back(glm::vec3(glX, glY - 15.0f / halfH, 0.0f));
		positions.push_back(glm::vec3(glX + 15.0f / halfW, glY, 0.0f));
		break;

	case 5: // Quad
		positions.push_back(glm::vec3(glX, glY, 0.0f));
		positions.push_back(glm::vec3(glX, glY - 15.0f / halfH, 0.0f));
		positions.push_back(glm::vec3(glX + 5.0f / halfW, glY - 10.0f / halfH, 0.0f));
		positions.push_back(glm::vec3(glX + 10.0f / halfW, glY - 10.0f / halfH, 0.0f));
		break;
	}

	curseur.setVertices(positions);
	curseur.Draw();
}

void Renderer::updateCursor()
{
	switch (typeCurseur)
	{
	case 1: // Point
	case 2: // Points
		curseur.setEpaisseurBordure(3);
		curseur.setTypePrimitive(GL_POINTS);
		break;

	case 3: // Croix
		curseur.setEpaisseurBordure(2);
		curseur.setTypePrimitive(GL_LINES);
		break;

	case 4: // Triangle
		curseur.setEpaisseurBordure(1);
		curseur.setTypePrimitive(GL_TRIANGLES);
		break;

	case 5: // Quad
		curseur.setEpaisseurBordure(1);
		curseur.setTypePrimitive(GL_TRIANGLE_FAN);
		break;
	}
}

void Renderer::deselectAllNodes()
{
	for (auto pair : selectedNodes)
		pair.first->setSelected(false);

	selectedNodes.clear();
}

std::shared_ptr<GroupObject> Renderer::castToGroupObject(std::shared_ptr<AbstractObject> obj)
{
	if (std::shared_ptr<GroupObject> casted = dynamic_pointer_cast<GroupObject>(obj))
	{
		return casted;
	}
	else
	{
		return nullptr;
	}
}

void Renderer::importerImage(string fichier)
{
	std::ifstream f(fichier);
	if (!f.good())
	{
		cout << "Fichier inexistant" << endl;
		return;
	}
	f.close();

	QuadObject quad(fichier);
	quad.Create(simpleTexShaderID);
	scene.addObject(std::make_shared<QuadObject>(quad));
}

void Renderer::importModel(string file)
{
	std::ifstream f(file);
	if (!f.good())
	{
		cout << "File not found" << endl;
		return;
	}
	f.close();

	ModelObject object;
	object.setModelToCreate(file);
	object.Create(modelShaderID);
	scene.addObject(std::make_shared<ModelObject>(object));
	//Resources/megalodon/megalodon.FBX
}

void Renderer::ajouterPtDessin(int x, int y)
{
	int w, h;
	SDL_GetWindowSize(window, &w, &h);

	float glX = x / (w / 2.0f) - 1.0f;
	float glY = -y / (h / 2.0f) + 1.0f;

	ptsDessin.push_back(glm::vec3(glX, glY, 0.0f));

	// Type de primitive
	GLenum typePrimitive = -1;
	string name = "Primitive";

	switch (formeADessiner)
	{
	case 0: // Point
		typePrimitive = GL_POINTS;
		name = "Point";
		break;

	case 1: // Ligne
		if (ptsDessin.size() >= 2)
		{
			typePrimitive = GL_LINES;
			name = "Ligne";
		}
		break;

	case 2: // Triangle
		if (ptsDessin.size() >= 3)
		{
			typePrimitive = GL_TRIANGLES;
			name = "Triangle";
		}
		break;

	case 3: // Rectangle
		if (ptsDessin.size() >= 2)
		{
			typePrimitive = GL_TRIANGLE_FAN;
			name = "Rectangle";
		}
		break;

	case 4: // Quad
		if (ptsDessin.size() >= 4)
		{
			typePrimitive = GL_TRIANGLE_FAN;
			name = "Quad";
		}
		break;

	case 5: // Smiley
		if (ptsDessin.size() >= 2)
			ajouterSmiley();
		break;

	case 6: // �toile
		if (ptsDessin.size() >= 2)
			ajouterEtoile();
		break;
	}

	// Ajout de primitive
	if (typePrimitive != -1)
	{
		PrimitiveObject primitive;
		primitive.Create(primitiveShaderID, name);
		primitive.setCouleurBordure(couleurBordure);
		primitive.setCouleurRemplissage(couleurRemplissage);
		primitive.setEpaisseurBordure(epaisseurBordure);
		primitive.setTypePrimitive(typePrimitive);

		// Lorsqu'on dessine un rectangle, on donne les coordonn�es de 2 sommets oppos�s
		// On doit donc ajouter 2 sommets au dessin
		if (formeADessiner == 3)
			ptsDessin = { ptsDessin[0], glm::vec3(ptsDessin[0].x, ptsDessin[1].y, 0.0f), ptsDessin[1], glm::vec3(ptsDessin[1].x, ptsDessin[0].y, 0.0f) };

		primitive.setVertices(ptsDessin);

		scene.addObject(std::make_shared<PrimitiveObject>(primitive));

		ptsDessin.clear();
	}
}

void Renderer::ajouterSmiley()
{
	const int nbPrimitives = 4;
	PrimitiveObject primitives[nbPrimitives];

	primitives[0].Create(primitiveShaderID, "Smiley");
	primitives[1].Create(primitiveShaderID, "Oeil gauche");
	primitives[2].Create(primitiveShaderID, "Oeil droit");
	primitives[3].Create(primitiveShaderID, "Sourire");

	for (int i = 0; i < nbPrimitives; i++)
	{
		primitives[i].setCouleurBordure(couleurBordure);

		if (i == 0)
			primitives[i].setCouleurRemplissage(couleurRemplissage);
		else
			primitives[i].setCouleurRemplissage(couleurBordure);

		primitives[i].setEpaisseurBordure(epaisseurBordure);
		primitives[i].setTypePrimitive(GL_TRIANGLE_FAN);
	}

	glm::vec3 topLeft = { min(ptsDessin[0].x, ptsDessin[1].x), max(ptsDessin[0].y, ptsDessin[1].y), 0.0f };
	glm::vec3 botRight = { max(ptsDessin[0].x, ptsDessin[1].x), min(ptsDessin[0].y, ptsDessin[1].y), 0.0f };
	float w = botRight.x - topLeft.x;
	float h = topLeft.y - botRight.y;

	std::vector<glm::vec3> vertices;

	// Carr� principal
	vertices = { topLeft, glm::vec3(topLeft.x, botRight.y, 0.0f), botRight, glm::vec3(botRight.x, topLeft.y, 0.0f) };
	primitives[0].setVertices(vertices);

	// Oeil gauche
	vertices = { 
		glm::vec3(topLeft.x + 0.125f * w, topLeft.y - 0.125f * h, 0.0f),
		glm::vec3(topLeft.x + 0.125f * w, topLeft.y - 0.375f * h, 0.0f),
		glm::vec3(topLeft.x + 0.375f * w, topLeft.y - 0.375f * h, 0.0f),
		glm::vec3(topLeft.x + 0.375f * w, topLeft.y - 0.125f * h, 0.0f)
	};
	primitives[1].setVertices(vertices);

	// Oeil droit
	vertices = {
		glm::vec3(botRight.x - 0.375f * w, topLeft.y - 0.125f * h, 0.0f),
		glm::vec3(botRight.x - 0.375f * w, topLeft.y - 0.375f * h, 0.0f),
		glm::vec3(botRight.x - 0.125f * w, topLeft.y - 0.375f * h, 0.0f),
		glm::vec3(botRight.x - 0.125f * w, topLeft.y - 0.125f * h, 0.0f)
	};
	primitives[2].setVertices(vertices);

	// Sourire
	vertices = {
		glm::vec3(topLeft.x + 0.125f * w, botRight.y + 0.375f * h, 0.0f),
		glm::vec3(topLeft.x + 0.25f * w, botRight.y + 0.125f * h, 0.0f),
		glm::vec3(botRight.x - 0.25f * w, botRight.y + 0.125f * h, 0.0f),
		glm::vec3(botRight.x - 0.125f * w, botRight.y + 0.375f * h, 0.0f)
	};
	primitives[3].setVertices(vertices);

	GroupObject smiley;
	for (int i = 0; i < nbPrimitives; i++)
		smiley.addObject(std::make_shared<PrimitiveObject>(primitives[i]));

	scene.addObject(std::make_shared<GroupObject>(smiley));

	ptsDessin.clear();
}

void Renderer::ajouterEtoile()
{
	PrimitiveObject primitive;
	primitive.Create(primitiveShaderID, "Etoile");
	primitive.setCouleurBordure(couleurBordure);
	primitive.setEpaisseurBordure(epaisseurBordure);
	primitive.setTypePrimitive(GL_LINES);
	
	glm::vec3 topLeft = { min(ptsDessin[0].x, ptsDessin[1].x), max(ptsDessin[0].y, ptsDessin[1].y), 0.0f };
	glm::vec3 botRight = { max(ptsDessin[0].x, ptsDessin[1].x), min(ptsDessin[0].y, ptsDessin[1].y), 0.0f };
	float w = botRight.x - topLeft.x;
	float h = topLeft.y - botRight.y;

	std::vector<glm::vec3> vertices;
	vertices.push_back(topLeft);
	vertices.push_back(botRight);
	vertices.push_back(glm::vec3(topLeft.x + 0.5f * w, topLeft.y, 0.0f));
	vertices.push_back(glm::vec3(topLeft.x + 0.5f * w, botRight.y, 0.0f));
	vertices.push_back(glm::vec3(botRight.x, topLeft.y, 0.0f));
	vertices.push_back(glm::vec3(topLeft.x, botRight.y, 0.0f));
	vertices.push_back(glm::vec3(botRight.x, botRight.y + 0.5f * h, 0.0f));
	vertices.push_back(glm::vec3(topLeft.x, botRight.y + 0.5f * h, 0.0f));

	primitive.setVertices(vertices);

	scene.addObject(std::make_shared<PrimitiveObject>(primitive));

	ptsDessin.clear();
}

void Renderer::eraseNodes()
{
	for (auto pair : selectedNodes)
	{
		pair.first->setSelected(false);
		if (pair.second != nullptr)
			pair.second->deleteObject(pair.first);
	}
	selectedNodes.clear();
}

void Renderer::groupNodes()
{
	if (selectedNodes.size() == 0)
		return;

	// Add all selected objects to a child group object
	GroupObject childGroup;
	for (auto pair : selectedNodes)
	{
		if (pair.second == nullptr)
			return;
		childGroup.addObject(pair.first);
	}

	// Add child group object to the first selected object's parent
	selectedNodes[0].second->addObject(std::make_shared<GroupObject>(childGroup));

	// Erase original nodes
	eraseNodes();
}
