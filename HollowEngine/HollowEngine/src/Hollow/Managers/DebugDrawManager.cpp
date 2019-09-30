#include <hollowpch.h>
#include "DebugDrawManager.h"
#include "RenderManager.h"
#include "ResourceManager.h"

#include "Hollow/Graphics/RenderData.h"

namespace Hollow
{
	void DebugDrawManager::Init()
	{

	}

	void DebugDrawManager::CleanUp()
	{

	}

	void DebugDrawManager::DebugLine()
	{
	}

	void DebugDrawManager::DebugCircle()
	{
	}

	void DebugDrawManager::DebugAxes()
	{
	}

	void DebugDrawManager::DebugSphere(glm::vec3 position, float scale)
	{
		glm::mat4 model = glm::mat4(1.0f);
		model = glm::translate(model, position);
		model = glm::scale(model, glm::vec3(scale));
		RenderData data;
		data.mpMeshes.push_back(ResourceManager::Instance().GetShape(Shapes::QUAD));
		data.mpModel = model;
		RenderManager::Instance().mDebugRenderData.push_back(data);
	}

	void DebugDrawManager::DebugCube()
	{
	}
}