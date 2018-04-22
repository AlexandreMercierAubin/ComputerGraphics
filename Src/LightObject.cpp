#include "LightObject.h"

LightObject::LightObject() 
{
	name = "Light";
}



void LightObject::setLight(const int &type,
							const glm::vec3 &ambientColor,
							const float &ambientIntensity,
							const glm::vec3 &diffuseColor,
							const float &diffuseIntensity,
							const glm::vec3 &specularColor,
							const float &specularIntensity,
							const float attenuation,
							const glm::vec3 &direction,
							const glm::vec3 &in_position,
							const float &coneAngle,
							bool volumetric,
							const glm::vec3 &volume)
{
	light.lightType = type;
	light.ambientColor = ambientColor;
	light.ambientIntensity = ambientIntensity;
	light.diffuseColor = diffuseColor;
	light.diffuseIntensity = diffuseIntensity;
	light.specularColor = specularColor;
	light.specularIntensity = specularIntensity;
	light.attenuation = attenuation;
	light.direction = direction;
	light.coneAngle = coneAngle;
	position=in_position;
	light.volumetric = volumetric;
	light.volume = volume;

	setRotationDegree(direction);
}

void LightObject::Draw(glm::mat4 &projection, glm::mat4 &view, glm::vec3 &camPos, const vector<Light*>& lights) 
{
	light.position = position;
}

AbstractObject::Light* LightObject::getLight() 
{
	return &light;
}

void LightObject::setRotationDegree(glm::vec3 rot)
{
	AbstractObject::setRotationDegree(rot);
	light.direction = glm::normalize(rotationDegree);
}

void LightObject::addRotationDegree(const glm::vec3 & rot)
{
	AbstractObject::addRotationDegree(rot);
	light.direction = glm::normalize(rotationDegree);
}

void LightObject::setRotationQuaternion(glm::quat quat)
{
	AbstractObject::setRotationQuaternion(quat);
	light.direction = glm::normalize(rotationDegree);
}

void LightObject::addRotationQuaternion(const glm::quat & quat)
{
	AbstractObject::addRotationQuaternion(quat);
	light.direction = glm::normalize(rotationDegree);
}

void LightObject::setColor(glm::vec4 Color)
{
	AbstractObject::setColor(Color);
	light.diffuseColor = Color;
}
