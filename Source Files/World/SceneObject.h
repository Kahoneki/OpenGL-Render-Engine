#ifndef SCENEOBJECT_H
#define SCENEOBJECT_H

#include <GLM/glm.hpp>

/*Virtual class - Must be created as derived class (Drawable, LightSource, or RenderSource)*/

class SceneObject
{
public:
	SceneObject();
	SceneObject(const char* _name, SceneObject* _parent, glm::vec3 worldPos);
	virtual ~SceneObject() = 0;

	//Setters
	virtual void setPosition(glm::vec3 pos) = 0; //Must be overloaded - setting the position of a render source will be different to a drawable (but all should modify worldPos)

	//Getters
	glm::vec3 getPosition() const;

protected:
	const char* name;
	SceneObject* parent;
	glm::vec3 worldPos;
};

#endif