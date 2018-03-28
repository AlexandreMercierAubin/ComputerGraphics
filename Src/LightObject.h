#pragma once
#include"AbstractObject.h"

class LightObject : public AbstractObject
{
public:
	LightObject();
	void setLight(const int &type,
		const glm::vec3 &ambientColor,
		const float &ambientIntensity,
		const glm::vec3 &diffuseColor,
		const float &diffuseIntensity,
		const glm::vec3 &specularColor,
		const float &specularIntensity,
		const float attenuation,
		const glm::vec3 &direction,
		const glm::vec3 &in_position);

	void Draw(glm::mat4 &projection, glm::mat4 &view, glm::vec3 &camPos, const vector<Light*>& lights);
	AbstractObject::Light* getLight();
private:
	AbstractObject::Light light;
};
