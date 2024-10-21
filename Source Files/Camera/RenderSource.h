#ifndef RENDERSOURCE_H
#define RENDERSOURCE_H

#include "../World/SceneObject.h"

class RenderSource : public SceneObject
{
public:
	RenderSource();
	virtual ~RenderSource();
	virtual void setPosition(glm::vec3 pos) override = 0;
};

#endif