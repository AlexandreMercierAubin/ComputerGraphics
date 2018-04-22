#include "Scene.h" 

Scene::Scene(void)
{
}

Scene::~Scene(void)
{
	glDeleteProgram(shaderID);
	glDeleteBuffers(1, &bufferID);
	glDeleteProgram(skyboxID);
	glDeleteProgram(tessellationShaderID);
	glDeleteBuffers(1, &tessellationShaderID);
	glDeleteProgram(texShaderID);
	glDeleteBuffers(1, &texShaderID);
}

void Scene::setupScene()
{
	
	direction = glm::vec3(0.0, 1.0, 1.0);
	position = glm::vec3(0.0, 0.0, 1.0);
	orientation = glm::vec3(0.0, 1.0, 0.0);
	yaw = -90;
	pitch = 0;

	uniformCouleur = glm::vec4(1.0, 1.0, 1.0, 1.0);

	projectionType = Perspective;
	setProjection(projectionType);
	MatView(view,false);

	//test remove when done; change for an abstractobject array
	Core::ShaderLoader loader;
	ModelShader modelShader;
	SkyboxShader skyboxShader;
	TexShader texShader;
	PrimitiveShader pShader;
	SimpleGPShader gpShader;
	TessellationShader tessShader;
	TessellationCEShader tessCeShader;

	shaderID = loader.CreateProgram(modelShader);
	skyboxID = loader.CreateProgram(skyboxShader);
	tessellationShaderID= loader.CreateProgramTess(tessShader, tessCeShader);
	texShaderID = loader.CreateProgram(texShader);
	GLuint primitiveShaderID= loader.CreateProgram(pShader);
	GLuint gpShaderID = loader.CreateProgram(gpShader);

	skybox.Create(skyboxID);
	objects = std::make_shared<GroupObject>();
	objects->addObject(make_shared<LightObject>());
	objects->getCastedObjectAt<LightObject>(0)->setLight(0, glm::vec3(1, 1, 1), 0.2f, glm::vec3(1, 1, 1), 1.0f, glm::vec3(1, 1, 1), 1.0f, 0.2f, glm::vec3(-1, -1, 0), glm::vec3(0, 0, 0),30);
	objects->addObject(make_shared<LightObject>());
	objects->getCastedObjectAt<LightObject>(1)->setLight(1, glm::vec3(1, 1, 1), 0.2f, glm::vec3(0, 1, 1), 1.0f, glm::vec3(1, 1, 1), 1.0f, 0.2f, glm::vec3(0, -1, 0), glm::vec3(-0.25, 7.5, 0),30);
	objects->addObject(make_shared<QuadObject>("Resources/Image/Small-mario.png"));
	objects->getObjectAt(2)->Create(texShaderID);
	objects->getObjectAt(2)->setPosition(glm::vec3(0, 0, -1));
	objects->addObject(make_shared<ModelObject>());
	objects->getObjectAt(3)->Create(shaderID);
	objects->getObjectAt(3)->setPosition(glm::vec3(0, -0.2f, 0));
	objects->addObject(make_shared<ModelObject>());
	objects->getObjectAt(4)->Create(shaderID);
	objects->getObjectAt(4)->setPosition(glm::vec3(0.5f, -0.2f, 0));
	objects->getObjectAt(4)->setColor(glm::vec4(1, 0, 0,1));
	objects->addObject(make_shared<ModelObject>());
	objects->getObjectAt(5)->Create(shaderID);
	objects->getObjectAt(5)->setPosition(glm::vec3(-0.5f, -0.2f, 0));
	objects->getObjectAt(5)->setColor(glm::vec4(0, 1, 0, 1));


	std::vector<glm::vec3> vertices;
	vertices.push_back(glm::vec3(-0.5, 0, 0));
	vertices.push_back(glm::vec3(0.0, -0.5, 0));
	vertices.push_back(glm::vec3(0.5, 0.0, 0));
	vertices.push_back(glm::vec3(0.0, 0.6, 0));
	vertices.push_back(glm::vec3(0.0, 0.0, 0));
	vertices.push_back(glm::vec3(0.0, 0.0, 0.5));
	vertices.push_back(glm::vec3(-1.0, 0.0, 0));
	vertices.push_back(glm::vec3(0.0, -1.0, 0));
	vertices.push_back(glm::vec3(0.0, 1.0, 0));


	glm::mat4 matrix;
	matrix[0][0] = 1.0f;	    matrix[0][1] = 0.0f;			matrix[0][2] = 0.0f;		matrix[0][3] = 0.0f;
	matrix[1][0] = 0.0f;		matrix[1][1] = 0.0f;			matrix[1][2] = 0.0f;		matrix[1][3] = 0.0f;
	matrix[2][0] = 0.0f;		matrix[2][1] = 0.0f;			matrix[2][2] = 1.0f;		matrix[2][3] = 0.0f;
	matrix[3][0] = 0.0f;		matrix[3][1] = 0.0f;			matrix[3][2] = 0.0f;		matrix[3][3] = 0.0f;

	objects->addObject(make_shared<MirrorObject>());
	objects->addObject(make_shared<MirrorObject>());

	//objects->getCastedObjectAt<ParametricSurfaceObject>(6)->setMatrix(matrix);

	/*objects->addObject(make_shared<ParametricCurveObject>());
	objects->getObjectAt(6)->Create(primitiveShaderID);
	objects->getCastedObjectAt<ParametricCurveObject>(6)->setParametricType(ParametricCurveObject::Bezier);
	objects->getCastedObjectAt<ParametricCurveObject>(6)->setNumLines(100);
	objects->getCastedObjectAt<ParametricCurveObject>(6)->setVertices(vertices);
	objects->getCastedObjectAt<ParametricCurveObject>(6)->setColor(glm::vec4(0, 0, 0, 1));
	objects->getCastedObjectAt<ParametricCurveObject>(6)->setEdgeColor(glm::vec4(0, 0, 0, 1));
	objects->getCastedObjectAt<ParametricCurveObject>(6)->setEdgeSize(5);*/


	setupLight();
}

void Scene::setupLight()
{
	lights.clear();
	objects->getLight(lights);
}

void Scene::setupMirrors()
{
	mirrors.clear();
	objects->getMirrors(mirrors);
}

void Scene::setProjection(PROJECTIONTYPE type, const float & angleOfView, const float & aspect, const float & near, const float &far)
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

void Scene::addYaw(GLfloat y)
{
	yaw += y;
	MatView(view, false);
}

void Scene::setPosition(glm::vec3 pos)
{
	position = pos;
	MatView(view, false);
}

void Scene::dollyZoom(float dolly,float zoom) 
{
	position = position + direction * dolly;
	viewAngle += zoom;
	setProjection(projectionType, glm::radians(viewAngle));
	MatView(view,false);
}

bool Scene::raycast(const Ray &ray, double &distance, std::shared_ptr<AbstractObject> &object)
{
	return objects->raycast(ray, distance, object);
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

void Scene::drawMirrors(int w, int h,bool DrawSkybox)
{
	glm::vec3 tempPlayerPos = position;
	GLuint fboM, fbo_textureM, rbo_depthM;

	glDisable(GL_CULL_FACE);

	glActiveTexture(GL_TEXTURE1);
	glGenTextures(1, &fbo_textureM);
	glBindTexture(GL_TEXTURE_2D, fbo_textureM);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(1);


	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
	glBindTexture(GL_TEXTURE_2D, 0);


	/* Depth buffer */
	glGenRenderbuffers(1, &rbo_depthM);
	glBindRenderbuffer(GL_RENDERBUFFER, rbo_depthM);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT16, w, h);
	glBindRenderbuffer(GL_RENDERBUFFER, 0);

	/* Framebuffer to link everything together */
	glGenFramebuffers(1, &fboM);
	glBindFramebuffer(GL_FRAMEBUFFER, fboM);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, fbo_textureM, 0);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, rbo_depthM);
	GLenum status;
	if ((status = glCheckFramebufferStatus(GL_FRAMEBUFFER)) != GL_FRAMEBUFFER_COMPLETE) {
		cout << "framebuffer error" << endl;
		return;
	}
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	for (unsigned int i = 0; i < mirrors.size(); ++i) 
	{
		//settint the position of the camera
		glm::vec3 mPos = mirrors[i]->getPosition();
		setPosition(mPos);

		//filling the framebuffer
		glBindFramebuffer(GL_FRAMEBUFFER, fboM);
		glClearColor(0, 0, 0, 1);// background
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		if (DrawSkybox)
			drawSkybox();

		drawScene();

		glBindFramebuffer(GL_FRAMEBUFFER, 0);


		//draw part
		glUseProgram(texShaderID);

		mirrors[i]->setTexture(fbo_textureM);

		mirrors[i]->drawMirror(projection, view, position, lights);
	}
	setPosition(tempPlayerPos);

	glDeleteRenderbuffers(1, &rbo_depthM);
	glDeleteTextures(1, &fbo_textureM);
	glDeleteFramebuffers(1, &fboM);

	glDisable(GL_TEXTURE_2D);
	glEnable(GL_CULL_FACE);
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
