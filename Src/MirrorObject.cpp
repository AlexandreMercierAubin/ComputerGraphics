#include "MirrorObject.h"

MirrorObject::MirrorObject() 
{
	name = "mirror";
	quad = make_shared<QuadObject>("Resources/Image/Small-mario.png");
}

void MirrorObject::setTexture(GLuint a)
{
	quad->setTexture(a);
}
void MirrorObject::drawMirror(glm::mat4 & projection, glm::mat4 & view, glm::vec3 & camPos, const vector<Light*>& lights)
{
	quad->Draw(projection, view, camPos, lights);
}
void MirrorObject::setPosition(glm::vec3 pos)
{
	quad->setPosition(pos);
}
glm::vec3 MirrorObject::getPosition()
{
	return quad->getPosition();
}
void MirrorObject::setRotationDegree(glm::vec3 rot)
{
	quad->setRotationDegree(rot);
}
void MirrorObject::addRotationDegree(const glm::vec3 & rot)
{
	quad->addRotationDegree(rot);
}
glm::vec3 MirrorObject::getRotationDegree()
{
	return quad->getRotationDegree();
}
void MirrorObject::addPosition(const glm::vec3 & pos)
{
	quad->addPosition(pos);
}
void MirrorObject::setScale(glm::vec3 scl)
{
	quad->setScale(scl);
}
void MirrorObject::addScale(const glm::vec3 & scl)
{
	quad->addScale(scl);
}
glm::vec3 MirrorObject::getScale()
{
	return quad->getScale();
}
void MirrorObject::setInverted(bool x, bool y)
{
	quad->setInverted(x, y);
}
void MirrorObject::setColor(glm::vec4 Color)
{
	quad->setColor(Color);
}
glm::vec4 MirrorObject::getColor()
{
	return quad->getColor();
}
void MirrorObject::Draw(glm::mat4 &projection, glm::mat4 &view, glm::vec3 &camPos, const vector<Light*>& lights)
{
	//nothing to be done
}

void MirrorObject::Create(GLuint & Program)
{
	quad->Create(program);
}


