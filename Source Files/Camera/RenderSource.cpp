#include "RenderSource.h"
#include "../../Shaders/shader.h"
#include <iostream>

RenderSource::RenderSource(const char* name, SceneObject* parent, Transform transform) : SceneObject(name, parent, transform)
{
	name = "New Render Source";
	parent = nullptr;
}

RenderSource::~RenderSource()
{
}