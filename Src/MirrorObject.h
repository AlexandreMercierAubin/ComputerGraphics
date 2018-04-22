#pragma once
#include"QuadObject.h"

class MirrorObject : public AbstractObject
{
private:
	std::shared_ptr<QuadObject> quad;
public:
	MirrorObject();
	void Draw(glm::mat4 &projection, glm::mat4 &view, glm::vec3 &camPos, const vector<Light*>& lights);

	void setTexture(GLuint tex);
	void drawMirror(glm::mat4 &projection, glm::mat4 &view, glm::vec3 &camPos, const vector<Light*>& lights);
	virtual void setPosition(glm::vec3 pos);
	virtual glm::vec3 getPosition();
	virtual void setRotationDegree(glm::vec3 rot);
	virtual void addRotationDegree(const glm::vec3 &rot);
	virtual  glm::vec3 getRotationDegree();
	virtual void addPosition(const glm::vec3 &pos);
};
