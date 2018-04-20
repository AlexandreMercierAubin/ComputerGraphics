#pragma once
#include "GroupObject.h"

void GroupObject::Draw(glm::mat4 &projection, glm::mat4 &view, glm::vec3 &camPos, const vector<Light*>& lights)
{
	//tranfers the draw call
	for (auto it = vObject.begin(); it != vObject.end(); ++it)
	{
		(*it)->Draw(projection, view,camPos,lights);
	}
}

void GroupObject::addObject(std::shared_ptr<AbstractObject> object) 
{
	vObject.push_back(object);
}

void GroupObject::deleteObjectAt(const int index)
{
	vObject.erase(vObject.begin() + index);
}

void GroupObject::deleteObject(std::shared_ptr<AbstractObject> object)
{
	for (unsigned int i = 0; i < vObject.size(); ++i)
	{
		if (vObject[i] == object)
		{
			deleteObjectAt(i);
			return;
		}
	}
}

unsigned int GroupObject::size()
{
	return vObject.size();
}

std::shared_ptr<AbstractObject> GroupObject::getObjectAt(const int index)
{
	return vObject.at(index);
}

void GroupObject::setPosition(glm::vec3 pos)
{
	AbstractObject::setPosition(pos);
	for (auto obj : vObject)
		obj->setPosition(pos);
}

void GroupObject::addPosition(const glm::vec3 &pos)
{
	AbstractObject::addPosition(pos);
	for (auto obj : vObject)
		obj->addPosition(pos);
}

void GroupObject::setRotationDegree(glm::vec3 rot)
{
	AbstractObject::setRotationDegree(rot);
	for (auto obj : vObject)
		obj->setRotationDegree(rot);
}

void GroupObject::addRotationDegree(const glm::vec3 &rot)
{
	AbstractObject::addRotationDegree(rot);
	for (auto obj : vObject)
		obj->addRotationDegree(rot);
}

void GroupObject::setRotationQuaternion(glm::quat quat)
{
	AbstractObject::setRotationQuaternion(quat);
	for (auto obj : vObject)
		obj->setRotationQuaternion(quat);
}

void GroupObject::addRotationQuaternion(const glm::quat &quat)
{
	AbstractObject::addRotationQuaternion(quat);
	for (auto obj : vObject)
		obj->addRotationQuaternion(quat);
}

void GroupObject::setScale(glm::vec3 scl)
{
	AbstractObject::setScale(scl);
	for (auto obj : vObject)
		obj->setScale(scl);
}

void GroupObject::addScale(const glm::vec3 &scl)
{
	AbstractObject::addScale(scl);
	for (auto obj : vObject)
		obj->addScale(scl);
}

void GroupObject::setColor(glm::vec4 Color)
{
	AbstractObject::setColor(Color);
	for (auto obj : vObject)
		obj->setColor(Color);
}

void GroupObject::getLight(std::vector<Light*>& lights)
{
	for (unsigned int i = 0; i < vObject.size();++i) 
	{
		if (isCastableAt<GroupObject>(i)) 
		{
			getCastedObjectAt<GroupObject>(i)->getLight(lights);
		}
		else if (isCastableAt<LightObject>(i))
		{
			lights.push_back(getCastedObjectAt<LightObject>(i)->getLight());
		}
	}
}

bool GroupObject::raycast(const Ray &ray, double &distance, glm::vec3 &normal, std::shared_ptr<AbstractObject> &object)
{
	bool intersect = false;
	distance = std::numeric_limits<double>::infinity();

	for (auto obj : vObject)
	{
		double dist = std::numeric_limits<double>::infinity();
		glm::vec3 n;
		std::shared_ptr<AbstractObject> o = obj;

		if (obj->raycast(ray, dist, n, o))
		{
			intersect = true;
			if (dist < distance)
			{
				distance = dist;
				normal = n;
				object = o;
			}
		}
	}

	return intersect;
}

GroupObject::GroupObject()
{
	name = "Groupe";
}

GroupObject::~GroupObject()
{
	vObject.clear();
}

