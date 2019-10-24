#include <hollowpch.h>
#include "DebugDrawManager.h"
#include "RenderManager.h"
#include "ResourceManager.h"

#include "Hollow/Graphics/Data/DebugRenderData.h"
#include "Hollow/Graphics/VertexArray.h"
#include "Hollow/Graphics/VertexBuffer.h"
#include "Hollow/Graphics/Mesh.h"
#include "Hollow/Graphics/Vertex.h"

namespace Hollow
{
	void DebugDrawManager::Init()
	{

	}

	void DebugDrawManager::CleanUp()
	{

	}
	
	void DebugDrawManager::DebugDirectionLine(glm::vec3 startPos, glm::vec3 endPos, glm::vec3 color)
	{
		glm::vec3 dirvec = endPos - startPos;
		glm::mat4 model = glm::mat4(1.0f);
		model = glm::translate(model, startPos);
		model *= GraphicsMath::RotationFromDirection(glm::normalize(dirvec));
		model = glm::scale(model, glm::vec3(length(dirvec)));
		std::vector<Mesh*> mesh;
		mesh.push_back(ResourceManager::Instance().GetShape(Shapes::DIRECTION_LINE));
		DebugRenderData data(mesh, model, GL_LINES, Shapes::DIRECTION_LINE, color);
		RenderManager::Instance().mDebugRenderData.push_back(data);
	}

	void DebugDrawManager::DebugRay(glm::vec3 startPos, glm::vec3 direction, glm::vec3 color)
	{
		glm::mat4 model = glm::mat4(1.0f);
		model = glm::translate(model, startPos);
		model *= GraphicsMath::RotationFromDirection(glm::normalize(direction));
		//model = glm::scale(model, glm::vec3(length(direction)));
		std::vector<Mesh*> mesh;
		mesh.push_back(ResourceManager::Instance().GetShape(Shapes::DIRECTION_LINE));
		DebugRenderData data(mesh, model, GL_LINES, Shapes::DIRECTION_LINE, color);
		RenderManager::Instance().mDebugRenderData.push_back(data);
	}

	void DebugDrawManager::DebugLine(glm::vec3 startPos, glm::vec3 endPos, glm::vec3 color)
	{
		glm::vec3 dirvec = endPos - startPos;
		glm::mat4 model = glm::mat4(1.0f);
		model = glm::translate(model, startPos);
		model *= GraphicsMath::RotationFromDirection(glm::normalize(dirvec));
		model = glm::scale(model, glm::vec3(length(dirvec)));
		std::vector<Mesh*> mesh;
		mesh.push_back(ResourceManager::Instance().GetShape(Shapes::LINE));
		DebugRenderData data(mesh, model, GL_LINES, Shapes::LINE, color);
		RenderManager::Instance().mDebugRenderData.push_back(data);
	}

	void DebugDrawManager::DebugCircle(glm::vec3 position, glm::vec2 scale, glm::vec3 color)
	{
		glm::mat4 model = glm::mat4(1.0f);
		model = glm::translate(model, position);
		model = glm::scale(model, glm::vec3(scale.x, scale.y, 1.0f));
		std::vector<Mesh*> mesh;
		mesh.push_back(ResourceManager::Instance().GetShape(Shapes::CIRCLE));
		DebugRenderData data(mesh, model, GL_LINE_LOOP, Shapes::CIRCLE, color);
		RenderManager::Instance().mDebugRenderData.push_back(data);
	}

	void DebugDrawManager::DebugQuad(glm::vec3 position, glm::vec2 scale, glm::vec3 color)
	{
		glm::mat4 model = glm::mat4(1.0f);
		model = glm::translate(model, position);
		model = glm::scale(model, glm::vec3(scale.x,scale.y,1.0f));
		std::vector<Mesh*> mesh;
		mesh.push_back(ResourceManager::Instance().GetShape(Shapes::QUAD));
		DebugRenderData data(mesh, model, GL_LINE_LOOP, Shapes::QUAD, color);
		RenderManager::Instance().mDebugRenderData.push_back(data);
	}

	void DebugDrawManager::DebugAxes(glm::vec3 position, glm::vec3 scale)
	{
		glm::mat4 model = glm::mat4(1.0f);
		model = glm::translate(model, position);
		model = glm::scale(model, scale);
		std::vector<Mesh*> mesh;
		mesh.push_back(ResourceManager::Instance().GetShape(Shapes::AXES));
		DebugRenderData data(mesh, model, GL_LINES, Shapes::AXES, COLOR_BLACK);
		RenderManager::Instance().mDebugRenderData.push_back(data);
	}

	void DebugDrawManager::DebugSphere(glm::vec3 position, glm::vec3 scale, glm::vec3 color)
	{
		glm::mat4 model = glm::mat4(1.0f);
		model = glm::translate(model, position);
		model = glm::scale(model, glm::vec3(scale));
		std::vector<Mesh*> mesh;
		mesh.push_back(ResourceManager::Instance().GetShape(Shapes::SPHERE));
		DebugRenderData data(mesh, model, GL_LINES, Shapes::SPHERE, color);
		RenderManager::Instance().mDebugRenderData.push_back(data);
	}

	void DebugDrawManager::DebugCube(glm::vec3 position, glm::vec3 scale, glm::vec3 color)
	{
		glm::mat4 model = glm::mat4(1.0f);
		model = glm::translate(model, position);
		model = glm::scale(model, scale);
		std::vector<Mesh*> mesh;
		mesh.push_back(ResourceManager::Instance().GetShape(Shapes::WIRECUBE));
		DebugRenderData data(mesh, model, GL_LINES, Shapes::WIRECUBE, color);
		RenderManager::Instance().mDebugRenderData.push_back(data);
	}
}