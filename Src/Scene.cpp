#include "Scene.h" 

Scene::Scene(void)
{
}

Scene::~Scene(void)
{
}




void MatRotation(GLuint program, float angle) // matrice de rotation
{
	GLuint MatRotation = glGetUniformLocation(program, "matRotation");

	glm::mat4 rotat;// rotation Y

	rotat[0][0] = cosf(angle); rotat[0][1] = 0.0f; rotat[0][2] = sinf(angle); rotat[0][3] = 0.0f;
	rotat[1][0] = 0.0f; rotat[1][1] = 1.0f; rotat[1][2] = 0.0f; rotat[1][3] = 0.0f;
	rotat[2][0] = -sinf(angle); rotat[2][1] = 0.0f; rotat[2][2] = cosf(angle); rotat[2][3] = 0.0f;
	rotat[3][0] = 0.0f; rotat[3][1] = 0.0f; rotat[3][2] = 0.0f; rotat[3][3] = 1.0f;

	glUniformMatrix4fv(MatRotation, 1, GL_TRUE, &rotat[0][0]);
}

void MatTrans(GLuint program, float x, float y, float z) // matrice de translation
{
	GLuint MatTrans = glGetUniformLocation(program, "matTranslation");

	glm::mat4 trans;

	trans = glm::translate(trans, glm::vec3(x, y, z));

	glUniformMatrix4fv(MatTrans, 1, GL_FALSE, &trans[0][0]);
}
void MatEch(GLuint program, float x, float y, float z) // matrice de translation
{
	GLuint MatEch = glGetUniformLocation(program, "matScale");

	glm::mat4 ech;

	ech = glm::scale(ech, glm::vec3(x, y, z));

	glUniformMatrix4fv(MatEch, 1, GL_FALSE, &ech[0][0]);
}


void Scene::uniformColor(GLuint &program, glm::vec4 &uniformColor)
{
	GLuint vecCouleur = glGetUniformLocation(program, "Color");
	glUniform4fv(vecCouleur, 1, &uniformColor[0]);
}

void Scene::uniformLight(GLuint &program, glm::vec3 &color, glm::vec3 &direction, float ambientIntensity, float diffuseIntensity)
{
	Light light;
	light.color = color;
	light.ambientIntensity = ambientIntensity;
	light.diffuseIntensity = diffuseIntensity;
	light.direction = direction;

	glUniform3fv(glGetUniformLocation(program,
		"structLight.color"), 1, &light.color[0]);
	glUniform1f(glGetUniformLocation(program,
		"structLight.ambientIntensity"),
		light.ambientIntensity);
	glUniform1f(glGetUniformLocation(program,
		"structLumiere. structLight.diffuseIntensity"),
		light.diffuseIntensity);
	glUniform3fv(glGetUniformLocation(program,
		"structLight.direction"), 1,
		&light.direction[0]);
}

GLuint Scene::loadCubemap(std::vector<char*> faces, GLint wrapS, GLint wrapT, GLint minFilter, GLint magFilter)
{
	GLuint textureID;
	SDL_Surface* image;
	GLint channels;
	GLenum type;

	//Générer la texture
	glGenTextures(1, &textureID);

	//Lier la texture à la cible texture 2D
	glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);

	//wrapping 
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
	//filtering 
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	for (GLuint i = 0; i < faces.size(); i++)
	{

		image = Model::loadImage(string(faces[i]));
		Model::getImageProperties(image, channels, type);

		//Associer l'information de l'image à la texture
		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, channels, image->w, image->h, 0, type, GL_UNSIGNED_BYTE, image->pixels);
		//Libérer l'image
		SDL_FreeSurface(image);
	}

	glBindTexture(GL_TEXTURE_CUBE_MAP, 0); //Délie la texture pour éviter d'être corrompue avec une autre

	return textureID; //retourne l'objet texture créé
}

void Scene::drawModel(GLuint &program, glm::mat4 &view,glm::mat4 &perspective, Model &model, glm::vec3 &matTranslation, glm::vec3 &matScale, glm::vec4 &color, float &lightIntensity, glm::vec3 &lightDirection)
{
	glUseProgram(program);
	GLuint MatView = glGetUniformLocation(program, "matView");
	glUniformMatrix4fv(MatView, 1, GL_FALSE, &view[0][0]);
	GLuint MatPerspective = glGetUniformLocation(program, "matPerspective");
	glUniformMatrix4fv(MatPerspective, 1, GL_FALSE, &perspective[0][0]);


	MatRotation(program, 0);
	uniformColor(program, color);
	uniformLight(program, glm::vec3(1.0, 1.0, 1.0), lightDirection, 0.3f, lightIntensity);


	//model
	MatTrans(program, (matTranslation).x, (matTranslation).y, (matTranslation).z);
	MatEch(program, (matScale).x, (matScale).y, (matScale).z);
	model.Draw(program);
}

void Scene::drawSkybox(glm::mat4 view, glm::mat4 perspective, GLuint program, glm::vec4 color)
{
	glUseProgram(program);
	uniformColor(program, color);
	GLuint MatView = glGetUniformLocation(program, "matView");
	glUniformMatrix4fv(MatView, 1, GL_FALSE, &view[0][0]);
	GLuint MatPerspective = glGetUniformLocation(program, "matPerspective");
	glUniformMatrix4fv(MatPerspective, 1, GL_FALSE, &perspective[0][0]);

	//Changer le test de profondeur
	glDepthMask(GL_FALSE);

	//Passer la texture
	glActiveTexture(GL_TEXTURE0);
	glUniform1i(glGetUniformLocation(program, "skybox"), 0);
	glBindTexture(GL_TEXTURE_CUBE_MAP, textureSkybox);

	//Dessiner le cube
	glBindVertexArray(vertexArraySkybox);
	glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);

	glDepthMask(GL_TRUE);
}

void Scene::createSkybox(GLfloat width, GLfloat height)
{

	glGenVertexArrays(1, &vertexArraySkybox);
	glBindVertexArray(vertexArraySkybox);

	createCube(width, height);

	std::vector<char*> faces;
	faces.push_back("Resources/Skybox/HandMadeSky_bk.tga");//rt
	faces.push_back("Resources/Skybox/HandMadeSky_bk.tga");//lf
	faces.push_back("Resources/Skybox/HandMadeSky_up.tga");//up
	faces.push_back("Resources/Skybox/HandMadeSky_dn.tga");//dn
	faces.push_back("Resources/Skybox/HandMadeSky_bk.tga");//bk
	faces.push_back("Resources/Skybox/HandMadeSky_bk.tga");//ft

	textureSkybox = loadCubemap(faces, GL_TEXTURE_WRAP_S, GL_TEXTURE_WRAP_T, GL_TEXTURE_MIN_FILTER, GL_TEXTURE_MAG_FILTER);

	glBindVertexArray(0);
}

void Scene::createCube(GLfloat width, GLfloat height)
{
	createVerticesCube(width / 2, height / 2);
	//creeTextureCube();

	//Création et remplissage du buffer d'indices de la base:
	GLuint IBOBase;
	glGenBuffers(1, &IBOBase);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBOBase);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indicesCube), indicesCube, GL_STATIC_DRAW);

}

void Scene::createTextureCube()
{
	//remplissage de la variable m_texture
	textureCube[0] = glm::vec2(0.0f, 0.0f);
	textureCube[1] = glm::vec2(0.0f, 1.0f);
	textureCube[2] = glm::vec2(1.0f, 0.0f);
	textureCube[3] = glm::vec2(1.0f, 1.0f);
	textureCube[4] = glm::vec2(1.0f, 0.0f);
	textureCube[5] = glm::vec2(1.0f, 1.0f);
	textureCube[6] = glm::vec2(0.0f, 0.0f);
	textureCube[7] = glm::vec2(0.0f, 1.0f);

	//Créer, remplir et activer le buffer de texture
	GLuint textureBuffer;
	glGenBuffers(1, &textureBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, textureBuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(textureCube), textureCube, GL_STATIC_DRAW);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(2);
}

void Scene::createVerticesCube(GLfloat width, GLfloat height)
{
	//Remplir la variable m_sommetsBase
	verticesCube[0] = glm::vec3(0 - width, 0 - height, 0 - width);
	verticesCube[1] = glm::vec3(0 - width, 0 + height, 0 - width);
	verticesCube[2] = glm::vec3(0 - width, 0 - height, 0 + width);
	verticesCube[3] = glm::vec3(0 - width, 0 + height, 0 + width);
	verticesCube[4] = glm::vec3(0 + width, 0 - height, 0 - width);
	verticesCube[5] = glm::vec3(0 + width, 0 + height, 0 - width);
	verticesCube[6] = glm::vec3(0 + width, 0 - height, 0 + width);
	verticesCube[7] = glm::vec3(0 + width, 0 + height, 0 + width);

	//Créer, remplir et activer le buffer de sommets
	GLuint vertexbuffer;
	glGenBuffers(1, &vertexbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(verticesCube), verticesCube, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(0);
}

void Scene::deleteScene()
{
	glDeleteVertexArrays(1, &vertexArraySkybox);
}

