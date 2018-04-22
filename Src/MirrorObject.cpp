#include "MirrorObject.h"

MirrorObject::MirrorObject() 
{
	name = "mirror";
	quad = make_shared<QuadObject>("Resources/Image/Small-mario.png");
	quad->Create();
}

void MirrorObject::setTexture(GLuint a)
{
	quad->setTexture(a);
}
void MirrorObject::drawMirror(glm::mat4 & projection, glm::mat4 & view, glm::vec3 & camPos, const vector<Light*>& lights)
{
	quad->DrawSimple(projection, view, camPos, lights);
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
void MirrorObject::Draw(glm::mat4 &projection, glm::mat4 &view, glm::vec3 &camPos, const vector<Light*>& lights)
{
	//nothing to be done
}


