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

	void DebugDrawManager::DebugQuad(glm::vec3 position, glm::vec2 scale)
	{
		glm::mat4 model = glm::mat4(1.0f);
		model = glm::translate(model, position);
		model = glm::scale(model, glm::vec3(scale.x,scale.y,1.0f));
		DebugRenderData data;
		data.mpMeshes.push_back(ResourceManager::Instance().GetShape(Shapes::QUAD));
		data.mpModel = model;
		data.mDrawCommand = GL_LINE_LOOP;
		RenderManager::Instance().mDebugRenderData.push_back(data);
	}

	void DebugDrawManager::DebugAxes()
	{
	}

	void DebugDrawManager::DebugSphere(glm::vec3 position, glm::vec3 scale)
	{
		glm::mat4 model = glm::mat4(1.0f);
		model = glm::translate(model, position);
		model = glm::scale(model, glm::vec3(scale));
		DebugRenderData data;
		data.mpMeshes.push_back(ResourceManager::Instance().GetShape(Shapes::SPHERE));
		data.mpModel = model;
		data.mDrawCommand = GL_LINES;
		RenderManager::Instance().mDebugRenderData.push_back(data);
	}

	void DebugDrawManager::DebugCube(glm::vec3 position, glm::vec3 scale)
	{
		glm::mat4 model = glm::mat4(1.0f);
		model = glm::translate(model, position);
		model = glm::scale(model, scale);
		DebugRenderData data;
		data.mpMeshes.push_back(ResourceManager::Instance().GetShape(Shapes::CUBE));
		data.mpModel = model;
		data.mDrawCommand = GL_LINE_LOOP;
		RenderManager::Instance().mDebugRenderData.push_back(data);
	}
}