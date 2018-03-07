#include "Scene.h" 

Scene::Scene(void)
{
}

Scene::~Scene(void)
{
}

void Scene::setupScene()
{
	direction = glm::vec3(0.0, 0.0, 1.0);
	uniformCouleur = glm::vec4(1.0, 1.0, 1.0, 1.0);

	position = glm::vec3(0.0, 0.0, 1.0);
	orientation = glm::vec3(0.0, 1.0, 0.0);
	yaw = -90;
	pitch = 0;

	perspective = glm::perspective(glm::radians(45.0f), 800.0f / 600.0f, 0.1f, 100.0f);
	view = MatView(false);

	//test remove when done; change for an abstractobject array
	Core::ShaderLoader loader;
	ModelShader modelShader;
	SkyboxShader skyboxShader;
	//g_requinModel = Model("Resources/megalodon/megalodon.FBX");
	shaderID = loader.CreateProgram(modelShader);
	skyboxID = loader.CreateProgram(skyboxShader);
	mySkybox.Create(skyboxID);
	quad.Create(shaderID);
}


glm::mat4 Scene::MatView(bool staticPos)
{
	glm::mat4 view;
	glm::vec3 front;
	glm::vec3 position;

	position.y = 0;

	front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
	front.y = sin(glm::radians(pitch));
	front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));

	direction = glm::normalize(front);

	if (staticPos)
	{
		position = glm::vec3(0.0, 0.0, 1.0);
	}
	else
	{
		position = position;
	}

	view = glm::lookAt(position, direction + position, orientation);
	return view;
}


void Scene::drawModel()
{
	//glUseProgram(program);
	//GLuint MatView = glGetUniformLocation(program, "matView");
	//glUniformMatrix4fv(MatView, 1, GL_FALSE, &view[0][0]);
	//GLuint MatPerspective = glGetUniformLocation(program, "matPerspective");
	//glUniformMatrix4fv(MatPerspective, 1, GL_FALSE, &perspective[0][0]);


	//MatRotation(program, 0);
	//uniformColor(program, color);
	//uniformLight(program, glm::vec3(1.0, 1.0, 1.0), lightDirection, 0.3f, lightIntensity);


	////model
	//MatTrans(program, (matTranslation).x, (matTranslation).y, (matTranslation).z);
	//MatEch(program, (matScale).x, (matScale).y, (matScale).z);
	//model.Draw(program);
}


void Scene::drawSkybox()
{
	mySkybox.Draw(perspective,view);
}


void Scene::deleteScene()
{
	glDeleteProgram(shaderID);
	glDeleteBuffers(1, &bufferID);
	
}

