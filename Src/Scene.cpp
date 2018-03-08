#include "Scene.h" 

Scene::Scene(void)
{
}

Scene::~Scene(void)
{
	glDeleteProgram(shaderID);
	glDeleteBuffers(1, &bufferID);
	glDeleteProgram(skyboxID);
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
	SimpleTexShader texShader;
	
	shaderID = loader.CreateProgram(modelShader);
	skyboxID = loader.CreateProgram(skyboxShader);
	GLuint texShaderID = loader.CreateProgram(texShader);

	skybox.Create(skyboxID);

	objects.addObject(make_shared<ModelObject>());
	objects.getObjectAt(0)->Create(shaderID);

	objects.addObject(make_shared<QuadObject>("Resources/Image/Small-mario.png"));
	objects.getObjectAt(1)->Create(texShaderID);

}

void Scene::addObject(shared_ptr<AbstractObject> object) 
{
	objects.addObject(object);
}

void Scene::refreshScene(KeyFlags flags)
{
	if (flags.flagRightMouse)
	{
		if (flags.flagUp)
		{
			position += mouvementSpeed * direction;
		}
		if (flags.flagDown)
		{
			position -= mouvementSpeed * direction;
		}
		if (flags.flagLeft)
		{
			glm::vec3 Droite = glm::cross(direction, orientation);
			position -= mouvementSpeed * glm::normalize(Droite);
		}
		if (flags.flagRight)
		{
			glm::vec3 Droite = glm::cross(direction, orientation);
			position += mouvementSpeed * glm::normalize(Droite);

		}
	}
	view = MatView(false);
}


glm::mat4 Scene::MatView(bool staticPos)
{
	glm::mat4 view;
	glm::vec3 front;
	glm::vec3 Position;

	position.y = 0;

	front.x = cos(glm::radians(yaw)) * cos(glm::radians(-pitch));
	front.y = sin(glm::radians(-pitch));
	front.z = sin(glm::radians(yaw)) * cos(glm::radians(-pitch));

	direction = glm::normalize(front);

	if (staticPos)
	{
		Position = glm::vec3(0.0, 0.0, 1.0);
	}
	else
	{
		Position = position;
	}

	view = glm::lookAt(Position, direction + Position, orientation);
	return view;
}


void Scene::drawScene()
{
	objects.Draw(perspective,view);
}

void Scene::mouseMotion(const unsigned int & timestamp, const unsigned int & windowID, const unsigned int & state, const int & x, const int & y, const int & xRel, const int & yRel)
{
	yaw += xRel;
	if (pitch + yRel*sensitivity > 89)
	{
		pitch = 89;
	}
	else if (pitch + yRel * sensitivity< -89)
	{
		pitch = -89;
	}
	else
	{
		pitch += yRel;
	}
	view = MatView(false);
}


void Scene::drawSkybox()
{
	skybox.Draw(perspective,view);
}




