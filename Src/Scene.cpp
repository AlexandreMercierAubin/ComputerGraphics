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
	position = glm::vec3(0.0, 0.0, 1.0);
	orientation = glm::vec3(0.0, 1.0, 0.0);
	yaw = -90;
	pitch = 0;

	uniformCouleur = glm::vec4(1.0, 1.0, 1.0, 1.0);

	perspective = MatPerspective(glm::radians(45.0f),1200/800 ,0.1f, 100.0f);
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

	objects = std::make_shared<GroupObject>();

	objects->addObject(make_shared<ModelObject>());
	objects->getObjectAt(0)->Create(shaderID);

	objects->addObject(make_shared<QuadObject>("Resources/Image/Small-mario.png"));
	objects->getObjectAt(1)->Create(texShaderID);

	//objects->addObject(make_shared<GroupObject>());
	//objects->getCastedObjectAt<GroupObject>(2)->addObject(make_shared<QuadObject>("Resources/Image/vodka.png"));
	//objects->getCastedObjectAt<GroupObject>(2)->getObjectAt(0)->Create(texShaderID);

	//cout << objects->isCastableAt<GroupObject>(2)<< endl;
	//cout << objects->isCastableAt<QuadObject>(2) << endl;
}

void Scene::addObject(shared_ptr<AbstractObject> object) 
{
	objects->addObject(object);
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
			glm::vec3 Right = glm::cross(direction, orientation);
			position -= mouvementSpeed * glm::normalize(Right);
		}
		if (flags.flagRight)
		{
			glm::vec3 Left = glm::cross(direction, orientation);
			position += mouvementSpeed * glm::normalize(Left);

		}
		view = MatView(false);
	}
}


glm::mat4 Scene::MatView(bool staticPos)
{
	glm::mat4 view;
	glm::vec3 front;
	glm::vec3 tempPos;

	position.y = 0;

	front.x = cosf(glm::radians(yaw)) * cosf(glm::radians(pitch));
	front.y = sinf(glm::radians(pitch));
	front.z = sinf(glm::radians(yaw)) * cosf(glm::radians(pitch));

	direction = glm::normalize(front);

	if (staticPos)
	{
		tempPos = glm::vec3(0.0, 0.0, 1.0);
	}
	else
	{
		tempPos = position;
	}

	view = glm::lookAt(tempPos, direction + tempPos, orientation);
	return view;
}


void Scene::drawScene()
{
	objects->Draw(perspective,view);
}

glm::mat4 Scene::MatPerspective(const float & angleOfView,const float &aspect, const float & near, const float &far)
{
	glm::mat4 pers;

	float xymax = near * tanf(angleOfView);
	float ymin = -xymax;
	float xmin = -xymax;

	float width = xymax - xmin;
	float height = xymax - ymin;

	float depth = far - near;
	float q = -(far + near) / depth;
	float qn = -2 * (far * near) / depth;

	float w = 2 * near / width;
	w = w / aspect;
	float h = 2 * near / height;

	float scale = 1 / tanf(angleOfView);
	pers[0][0] = w; pers[0][1] = 0.0f; pers[0][2] = 0.0f; pers[0][3] = 0.0f;
	pers[1][0] = 0.0f; pers[1][1] = h; pers[1][2] = 0.0f; pers[1][3] = 0.0f;
	pers[2][0] = 0.0f; pers[2][1] = 0.0f; pers[2][2] = q; pers[2][3] = -1;
	pers[3][0] = 0.0f; pers[3][1] = 0.0f; pers[3][2] = qn; pers[3][3] = 0;

	return pers;
}

void Scene::mouseMotion(const unsigned int & timestamp, const unsigned int & windowID, const unsigned int & state, const int & x, const int & y, const int & xRel, const int & yRel)
{
	yaw += xRel;
	if (pitch - yRel*sensitivity > 89)
	{
		pitch = 89;
	}
	else if (pitch - yRel * sensitivity< -89)
	{
		pitch = -89;
	}
	else
	{
		pitch -= yRel;
	}
	view = MatView(false);
}


void Scene::drawSkybox()
{
	skybox.Draw(perspective,view);
}

std::shared_ptr<GroupObject> Scene::getObjects()
{
	return objects;
}
