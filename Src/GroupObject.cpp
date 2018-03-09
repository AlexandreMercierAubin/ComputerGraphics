#pragma once
#include "GroupObject.h"

void GroupObject::Draw(glm::mat4 &perspective, glm::mat4 &view)
{
	//tranfers the draw call
	for (auto it = vObject.begin(); it != vObject.end(); ++it)
	{
		(*it)->Draw(perspective, view);
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

GroupObject::GroupObject()
{
	name = "Groupe";
}

GroupObject::~GroupObject()
{
	vObject.clear();
}

