#pragma once
#include "AbstractObject.h"
#include "Model.h"
#include <memory>

class GroupObject : public AbstractObject
{
public:
	virtual void Draw(glm::mat4 &perspective,glm::mat4 &view);
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

	GroupObject();
	~GroupObject();
private:
	std::vector<std::shared_ptr<AbstractObject>> vObject;
};
