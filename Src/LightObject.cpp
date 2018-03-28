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
							const glm::vec3 &in_position)
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
	position=in_position;
}

void LightObject::Draw(glm::mat4 &projection, glm::mat4 &view, glm::vec3 &camPos, const vector<Light*>& lights) 
{
	light.position = position;
}

AbstractObject::Light* LightObject::getLight() 
{
	return &light;
}



