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

	projectionType = Perspective;
	setProjection(projectionType,glm::radians(viewAngle),1200/800 ,0.1f, 100.0f);
	MatView(view,false);

	//test remove when done; change for an abstractobject array
	Core::ShaderLoader loader;
	ModelShader modelShader;
	SkyboxShader skyboxShader;
	TexShader texShader;
	
	shaderID = loader.CreateProgram(modelShader);
	skyboxID = loader.CreateProgram(skyboxShader);
	GLuint texShaderID = loader.CreateProgram(texShader);

	skybox.Create(skyboxID);

	objects = std::make_shared<GroupObject>();
	objects->addObject(make_shared<LightObject>());
	objects->addObject(make_shared<QuadObject>("Resources/Skybox/HandMadeSky_bk2.tga"));
	objects->getObjectAt(1)->Create(texShaderID);
	objects->getObjectAt(1)->setPosition(glm::vec3(0, 0, -2));
	objects->getObjectAt(1)->setScale(glm::vec3(5, 5, 0));
	objects->addObject(make_shared<QuadObject>("Resources/Image/Small-mario.png"));
	objects->getObjectAt(2)->Create(texShaderID);
	objects->getObjectAt(2)->setPosition(glm::vec3(0, 0, -1));
	objects->addObject(make_shared<ModelObject>());
	objects->getObjectAt(3)->Create(shaderID);
	objects->getObjectAt(3)->setPosition(glm::vec3(0, -0.2f, 0));

	setupLight();
}

void Scene::setupLight()
{
	lights.clear();
	objects->getLight(lights);
}

void Scene::setProjection(PROJECTIONTYPE type,const float & angleOfView, const float & aspect, const float & near, const float &far)
{
	projectionType = type;
	switch (type)
	{
	case Perspective:
		MatPerspective(projection, angleOfView, aspect, near, far);
		break;
	case Orthographic:
		MatOrthographic(projection, angleOfView, aspect, near, far);
		break;
	case InversePerspective:
		MatInversePerspective(projection, angleOfView, aspect, near, far);
		break;
	}
	
}

void Scene::dollyZoom(float dolly,float zoom) 
{
	position = position + direction * dolly;
	viewAngle += zoom;
	setProjection(projectionType, glm::radians(viewAngle), 1200 / 800, 0.1f, 100.0f);
	MatView(view,false);
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
		MatView(view,false);
	}
}


void Scene::MatView(glm::mat4 &matView, bool staticPos)
{
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

	matView = glm::lookAt(tempPos, direction + tempPos, orientation);
}


void Scene::drawScene()
{
	objects->Draw(projection,view,position, lights);
}

void Scene::MatPerspective(glm::mat4 &proj,const float & angleOfView,const float &aspect, const float & near, const float &far)
{
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
	proj[0][0] = w; proj[0][1] = 0.0f; proj[0][2] = 0.0f; proj[0][3] = 0.0f;
	proj[1][0] = 0.0f; proj[1][1] = h; proj[1][2] = 0.0f; proj[1][3] = 0.0f;
	proj[2][0] = 0.0f; proj[2][1] = 0.0f; proj[2][2] = q; proj[2][3] = -1.0f;
	proj[3][0] = 0.0f; proj[3][1] = 0.0f; proj[3][2] = qn; proj[3][3] = 0.0f;

}

void Scene::MatInversePerspective(glm::mat4 &proj, const float & angleOfView, const float &aspect, const float & near, const float &far)
{
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
	proj[0][0] = w; proj[0][1] = 0.0f; proj[0][2] = 0.0f; proj[0][3] = 0.0f;
	proj[1][0] = 0.0f; proj[1][1] = h; proj[1][2] = 0.0f; proj[1][3] = 0.0f;
	proj[2][0] = 0.0f; proj[2][1] = 0.0f; proj[2][2] = q; proj[2][3] = -1.0f;
	proj[3][0] = 0.0f; proj[3][1] = 0.0f; proj[3][2] = qn; proj[3][3] = 0.0f;

	proj = glm::inverse(proj);
}

void Scene::MatOrthographic(glm::mat4 & proj, const float & angleOfView, const float & aspect, const float & near, const float &far)
{
	float xymax = near * tanf(angleOfView);
	float ymin = -xymax;
	float xmin = -xymax;

	float width = xymax - xmin;
	float height = xymax - ymin;

	float depth = far - near;
	float q = -2 / depth;
	float qn = -(far + near) / depth;

	float w = 2 / width;
	//w = w / aspect;
	float h = 2 / height;

	float scale = 1 / tanf(angleOfView);
	proj[0][0] = w; proj[0][1] = 0.0f; proj[0][2] = 0.0f; proj[0][3] = 0.0f;
	proj[1][0] = 0.0f; proj[1][1] = h; proj[1][2] = 0.0f; proj[1][3] = 0.0f;
	proj[2][0] = 0.0f; proj[2][1] = 0.0f; proj[2][2] = q; proj[2][3] = 0.0f;
	proj[3][0] = 0.0f; proj[3][1] = 0.0f; proj[3][2] = qn; proj[3][3] = 1.0f;
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
	MatView(view,false);
}


void Scene::drawSkybox()
{
	skybox.Draw(projection,view,position,lights);
}

std::shared_ptr<GroupObject> Scene::getObjects()
{
	return objects;
}
