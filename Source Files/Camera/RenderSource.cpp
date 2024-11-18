#include "RenderSource.h"

RenderSource::RenderSource(const char* name, SceneObject* parent, Transform transform) : SceneObject(name, parent, transform)
{
	name = "New Render Source";
	parent = nullptr;
}

RenderSource::~RenderSource()
{
}
