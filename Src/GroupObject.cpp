#pragma once
#include "GroupObject.h"

void GroupObject::Draw(glm::mat4 &perspective, glm::mat4 &view)
{

}

void GroupObject::addObject(std::shared_ptr<AbstractObject> object) 
{
	vObject.push_back(object);
}

GroupObject::~GroupObject()
{
	vObject.clear();
}
