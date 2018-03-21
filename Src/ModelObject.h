#pragma once
#include "AbstractObject.h"
#include "Model.h"

class ModelObject: public AbstractObject
{
public:
	virtual void Create(GLuint &Program);
	virtual void Draw(glm::mat4 &projection,glm::mat4 &view);
	void setModelToCreate(string path);
	//virtual void SetColor(glm::vec4 color);
private:
	string modelPath = "Resources/Goblin/fbx/Android_Man_Model.fbx";
	Model model;
};
