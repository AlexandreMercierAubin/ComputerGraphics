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
	g_requinModel = Model("Resources/megalodon/megalodon.FBX");
	shaderID = loader.CreateProgram(modelShader);
	skyboxID = loader.CreateProgram(skyboxShader);
	vObject[0]->Create(skyboxID);
}

void Scene::addObject(shared_ptr<AbstractObject> object) 
{
	vObject.push_back(object);
}


glm::mat4 Scene::MatView(bool staticPos)
{
	glm::mat4 view;
	glm::vec3 front;
	glm::vec3 Position;

	position.y = 0;

	front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
	front.y = sin(glm::radians(pitch));
	front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));

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
	for (auto it = ++vObject.begin(); it != vObject.end(); ++it)
	{
		(*it)->Draw(perspective, view);
	}
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
	vObject[0]->Draw(perspective,view);
}


void Scene::deleteScene()
{
	glDeleteProgram(shaderID);
	glDeleteBuffers(1, &bufferID);
	glDeleteProgram(skyboxID);

	//deletes vObject safely
	for (auto  it = vObject.begin(); it != vObject.end(); ++it)
	{
		(*it)->Delete();
	}
	vObject.clear();
}

