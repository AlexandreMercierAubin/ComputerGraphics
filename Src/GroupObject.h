#pragma once
#include "AbstractObject.h"
#include "LightObject.h"
#include "MirrorObject.h"
#include "Model.h"
#include <memory>

class GroupObject : public AbstractObject
{
public:
	virtual void Draw(glm::mat4 &projection, glm::mat4 &view, glm::vec3 &camPos, const vector<Light*>& lights);
	void addObject(std::shared_ptr<AbstractObject> object);
	void deleteObjectAt(const int index);
	void deleteObject(std::shared_ptr<AbstractObject> object);
	unsigned int size();
	std::shared_ptr<AbstractObject> getObjectAt(const int index);

	template <class abObject>
	std::shared_ptr<abObject> getCastedObjectAt(const int index)
	{
		if (std::shared_ptr<abObject> casted = dynamic_pointer_cast<abObject>(vObject[index]))
		{
			return casted;
		}
		else 
		{
			return NULL;
		}
	}

	template <class abObject>
	bool isCastableAt(const int index)
	{
		if (abObject* casted = dynamic_cast<abObject*>(vObject[index].get()))
		{
			return true;
		}
		else
		{
			return false;
		}
	}

	virtual void setPosition(glm::vec3 pos);
	virtual void addPosition(const glm::vec3 &pos);
	virtual void setRotationDegree(glm::vec3 rot);
	virtual void addRotationDegree(const glm::vec3 &rot);
	virtual void setRotationQuaternion(glm::quat quat);
	virtual void addRotationQuaternion(const glm::quat &quat);
	virtual void setScale(glm::vec3 scl);
	virtual void addScale(const glm::vec3 &scl);
	virtual void setColor(glm::vec4 Color);

	virtual void getLight(std::vector<Light*>& lights);
	virtual void getMirrors(std::vector<std::shared_ptr<MirrorObject>>& mirrors);

	virtual bool raycast(const Ray &ray, double &distance, std::shared_ptr<AbstractObject> &object);

	GroupObject();
	~GroupObject();
private:
	std::vector<std::shared_ptr<AbstractObject>> vObject;
};
