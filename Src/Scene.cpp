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

	shaderID = loader.CreateProgram(modelShader);
	skyboxID = loader.CreateProgram(skyboxShader);
	texShaderID = loader.CreateProgram(texShader);
	GLuint primitiveShaderID= loader.CreateProgram(pShader);
	GLuint gpShaderID = loader.CreateProgram(gpShader);

	skybox.Create(skyboxID);
	objects = std::make_shared<GroupObject>();
	objects->addObject(make_shared<LightObject>());
	objects->getCastedObjectAt<LightObject>(0)->setLight(0, glm::vec3(1, 1, 1), 0.2f, glm::vec3(1, 1, 1), 1.0f, glm::vec3(1, 1, 1), 1.0f, 0.2f, glm::vec3(-1, -1, 0), glm::vec3(0, 0, 0),30,false,glm::vec3(0,0,0));
	objects->addObject(make_shared<LightObject>());
	objects->getCastedObjectAt<LightObject>(1)->setLight(1, glm::vec3(1, 1, 1), 0.2f, glm::vec3(0, 1, 1), 1.0f, glm::vec3(1, 1, 1), 1.0f, 0.2f, glm::vec3(0, -1, 0), glm::vec3(-0.25, 7.5, 0),30,false, glm::vec3(0, 0, 0));
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


	setupLight();
	setupMirrors();
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

void Scene::addPitch(GLfloat z)
{
	pitch += z;
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

int Scene::formatColorComponent(double value)
{
	value = glm::pow(glm::clamp(value, 0.0, 1.0), gammaCorrection);
	value = value * 255 + 0.5;
	return static_cast<int>(value);
}

void Scene::saveImageFile(const int &width, const int &height, glm::vec4* pixels, const string &fileName)
{
	std::cout << "Writing to file..." << std::endl;

	std::ofstream file;
	file.open(fileName + ".ppm", std::ios::out);
	
	// Entête du fichier pour une image avec un espace de couleur RGB 24 bits
	file << "P3\n";

	// Largeur et hauteur de l'image sur la seconde ligne de l'entête
	file << width << ' ' << height << '\n';

	// Valeur maximale de l'espace de couleur sur la troisième ligne de l'entête
	file << "255\n";

	// Écriture des pixels dans le fichier image
	for (int index = 0; index < width * height; ++index)
	{
		file << static_cast<std::uint32_t>(formatColorComponent(pixels[index].x)) << " ";
		file << static_cast<std::uint32_t>(formatColorComponent(pixels[index].y)) << " ";
		file << static_cast<std::uint32_t>(formatColorComponent(pixels[index].z)) << " ";
	}

	file.close();
	std::cout << "File saved" << std::endl;
}

void Scene::renderRaycast(const int &width, const int &height, const int &rayPerPixel, const int &depth, const string &fileName)
{
	std::cout << "Render start" << std::endl;
	float progress = 0.0f;

	glm::vec4 *pixels = new glm::vec4[width * height];
	for (int i = 0; i < width * height; ++i)
		pixels[i] = glm::vec4(0.0f, 0.0f, 0.0f, 0.0f);

	double proportion = 1.0 / rayPerPixel;

	glm::vec3 zAxis = orientation;
	glm::vec3 xAxis(width * viewAngle / height, 0.0f, 0.0f);
	glm::vec3 yAxis = glm::normalize(glm::cross(xAxis, zAxis)) * viewAngle;

	for (int y = 0; y < height; ++y)
	{
		progress = 100.0f * y / (height - 1.0f);
		std::cout << progress << "%" << std::endl;

		for (int x = 0; x < width; ++x)
		{
			int index = y * width + x;
			for (int sy = 0; sy < 2; ++sy)
			{
				for (int sx = 0; sx < 2; ++sx)
				{
					glm::vec4 radiance(0.0f, 0.0f, 0.0f, 0.0f);
					for (int s = 0; s < rayPerPixel; ++s)
					{
						double r1 = 2.0 * random01(rng);
						double dx = r1 < 1.0 ? glm::sqrt(r1) - 1.0 : 1.0 - glm::sqrt(2.0 - r1);
						double r2 = 2.0 * random01(rng);
						double dy = r2 < 1.0 ? glm::sqrt(r2) - 1.0 : 1.0 - glm::sqrt(2.0 - r2);
						double tempX = ((sx + 0.5 + dx) / 2.0 + x) / width - 0.5;
						double tempY = ((sy + 0.5 + dy) / 2.0 + y) / height - 0.5;
						
						glm::vec3 direction = glm::vec3(xAxis.x * tempX, xAxis.y * tempX, xAxis.z * tempX) +
							glm::vec3(yAxis.x * tempY, yAxis.y * tempY, yAxis.z * tempY) + zAxis;

						glm::vec4 tempRadiance = computeRadiance(Ray(position, glm::normalize(direction)), 0, depth);
						radiance += glm::vec4(tempRadiance.x * proportion, tempRadiance.y * proportion, tempRadiance.z * proportion, tempRadiance.w * proportion);
					}
					pixels[index] += glm::clamp(radiance, 0.0f, 1.0f);
				}
			}
		}
	}
	std::cout << "Render complete" << std::endl;
	saveImageFile(width, height, pixels, fileName);
}

bool Scene::raycast(const Ray &ray, double &distance, glm::vec3 &normal, std::shared_ptr<AbstractObject> &object)
{
	return objects->raycast(ray, distance, normal, object);
}

glm::vec4 Scene::computeRadiance(const Ray &ray, const int &depth, const int &maxDepth)
{
	double distance;
	glm::vec3 normal;
	std::shared_ptr<AbstractObject> obj;

	if (!raycast(ray, distance, normal, obj))
		return glm::vec4();

	// Coordonnées du point d'intersection
	glm::vec3 x = ray.origin + glm::vec3(ray.direction.x * distance, ray.direction.y * distance, ray.direction.z * distance);

	// Ajustement de la normale
	normal = glm::normalize(normal);
	if (glm::dot(normal, ray.direction) > 0)
		normal = glm::vec3(normal.x * -1, normal.y * -1, normal.z * -1);

	glm::vec4 f = obj->getColor();
	glm::vec4 color = glm::vec4(0.0f, 0.0f, 0.0f, 0.0f);

	for (auto light : lights)
	{
		if (light->lightType == 0) // Directionnelle
		{
			glm::vec4 vAmbient = glm::vec4(light->ambientColor * light->ambientIntensity, 1.0f);
			float diffuseFactor = glm::dot(normal, -light->direction);

			glm::vec4 diffuseColor;
			if (diffuseFactor > 0)
				diffuseColor = glm::vec4(light->diffuseColor * light->diffuseIntensity * diffuseFactor, 1.0f);
			else
				diffuseColor = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);

			color += f * (vAmbient + diffuseColor);
		}
		else if (light->lightType == 1) // Ponctuelle
		{
			glm::vec3 vAmbient = light->ambientColor * light->ambientIntensity * glm::vec3(f);
			glm::vec3 surfaceToLight = light->position - x;
			glm::vec3 surfaceToCamera = glm::normalize(this->position - x);

			float attenuation = 1.0f / (1.0f + light->attenuation * glm::pow(glm::length(surfaceToLight), 2));
			surfaceToLight = glm::normalize(surfaceToLight);

			color += makeLightPoint(f, obj->getShininess(), normal, vAmbient, surfaceToLight, surfaceToCamera, attenuation, light);
		}
		else if (light->lightType == 2) // Projecteur
		{
			glm::vec3 vAmbient = light->ambientColor * light->ambientIntensity * glm::vec3(f);
			glm::vec3 surfaceToLight = light->position - x;
			glm::vec3 surfaceToCamera = glm::normalize(this->position - x);

			float attenuation = 1.0f / (1.0f + light->attenuation * glm::pow(glm::length(surfaceToLight), 2));
			surfaceToLight = glm::normalize(surfaceToLight);

			float lightToSurfaceAngle = glm::degrees(glm::acos(glm::dot(-surfaceToLight, glm::normalize(-light->direction))));
			if (lightToSurfaceAngle < light->coneAngle)
				color += makeLightPoint(f, obj->getShininess(), normal, vAmbient, surfaceToLight, surfaceToCamera, attenuation, light);
		}
	}
	float blendFactor = 1.0f;
	if (color.w > 1.0f)
		blendFactor = 1.0f / color.w;
	color = glm::vec4(color.x * blendFactor, color.y * blendFactor, color.z * blendFactor, color.w * blendFactor);

	if (depth < maxDepth)
	{
		glm::vec4 temp = computeRadiance(Ray(x, glm::reflect(direction, normal)), depth + 1, maxDepth);
		color += glm::vec4(temp.x * 0.5f, temp.y * 0.5f, temp.z * 0.5f, temp.w * 0.5f);
	}

	return color;
}

glm::vec4 Scene::makeLightPoint(const glm::vec4 &color, const float &shininess, const glm::vec3 &normal, glm::vec3 &vAmbient, 
	const glm::vec3 &surfaceToLight, const glm::vec3 &surfaceToCamera, const float &attenuation, AbstractObject::Light *light)
{
	glm::vec3 diffuseColor(0.0f, 0.0f, 0.0f);
	float diffuseFactor = glm::max(0.0f, glm::dot(normal, surfaceToLight));
	if (diffuseFactor > 0)
		diffuseColor = glm::vec3(light->diffuseColor * light->diffuseIntensity * diffuseFactor);

	float specularFactor = 0.0f;
	if (diffuseFactor > 0.0f)
		specularFactor = glm::pow(glm::max(0.0f, glm::dot(surfaceToCamera, glm::reflect(-surfaceToLight, normal))), shininess);
	glm::vec3 specularColor(specularFactor * light->specularIntensity * light->specularColor);

	glm::vec3 linearColor = vAmbient + attenuation * (diffuseColor + specularColor);
	glm::vec3 gamma(1.0f / 2.2f);

	return glm::vec4(glm::pow(linearColor, gamma), color.w);
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
		setPosition(-mPos);

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
	skybox.Draw(projection, view, position, lights);
}

std::shared_ptr<GroupObject> Scene::getObjects()
{
	return objects;
}

void Scene::setFog(bool value)
{
	objects->setFog(value);
	skybox.setFog(value);
}

bool Scene::getFog()
{
	return objects->getFog();
}
