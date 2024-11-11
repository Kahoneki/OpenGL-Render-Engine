#include "RenderSource.h"

RenderSource::RenderSource(const char* name, SceneObject* parent, glm::vec3 worldPos) : SceneObject(name, parent, worldPos)
{
	name = "New Render Source";
	parent = nullptr;
}

RenderSource::~RenderSource()
{
}
