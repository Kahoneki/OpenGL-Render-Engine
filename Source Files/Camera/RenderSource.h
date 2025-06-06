#ifndef RENDERSOURCE_H
#define RENDERSOURCE_H

#include "../World/SceneObject.h"
#include "../World/Drawable/Special/PostprocessOverlay.h"

class RenderSource : public SceneObject
{
public:
	RenderSource(const char* name, SceneObject* parent, Transform transform);
	virtual ~RenderSource();
	virtual void setPosition(glm::vec3 pos) override = 0;

	PostprocessOverlay postprocessOverlay;
};

#endif