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
	std::shared_ptr<AbstractObject> getObjectAt(const int index);

	template <class abObject>
	abObject* getCastedObjectAt(const int index)
	{
		if (abObject* casted = dynamic_cast<abObject*>(vObject[index].get()))
		{
			return casted;
		}
		else 
		{
			return NULL;
		}
	}
	~GroupObject();
private:
	std::vector<std::shared_ptr<AbstractObject>> vObject;
};
