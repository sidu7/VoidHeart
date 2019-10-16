#include <hollowpch.h>
#include "DebugDrawManager.h"
#include "RenderManager.h"
#include "ResourceManager.h"

#include "Hollow/Graphics/RenderData.h"
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
	
	void DebugDrawManager::DebugLine(glm::vec3 startPos, glm::vec3 endPos, glm::vec3 color)
	{
		glm::vec3 dirvec = endPos - startPos;
		glm::vec3 nordir = glm::normalize(dirvec);
		glm::mat4 model = glm::mat4(1.0f);
		model = glm::translate(model, startPos);

		float angle = std::atan2(nordir.y, nordir.x);
		glm::mat4 glmrotXY = glm::rotate(angle, glm::vec3(0.0f, 0.0f, 1.0f));
		// Find the angle with the xy with plane (0, 0, 1); the - there is because we want to 
		// 'compensate' for that angle (a 'counter-angle')
		float angleZ = -std::asin(nordir.z);
		// Make the matrix for that, assuming that Y is your 'side' vector; makes the model 'pitch'
		glm::mat4 glmrotZ = glm::rotate(angleZ, glm::vec3(0.0f, 1.0f, 0.0f));

		model *= glmrotXY * glmrotZ;
		model = glm::scale(model, glm::vec3(length(dirvec)));
		DebugRenderData data;
		data.mpMeshes.push_back(ResourceManager::Instance().GetShape(Shapes::DIRECTION_LINE));
		data.mpModel = model;
		data.mDrawCommand = GL_LINES;
		data.mType = DebugShape::DEBUGLINE;
		data.mColor = color;
		RenderManager::Instance().mDebugRenderData.push_back(data);

	}

	void DebugDrawManager::DebugCircle()
	{

	}

	void DebugDrawManager::DebugQuad(glm::vec3 position, glm::vec2 scale, glm::vec3 color)
	{
		glm::mat4 model = glm::mat4(1.0f);
		model = glm::translate(model, position);
		model = glm::scale(model, glm::vec3(scale.x,scale.y,1.0f));
		DebugRenderData data;
		data.mpMeshes.push_back(ResourceManager::Instance().GetShape(Shapes::QUAD));
		data.mpModel = model;
		data.mDrawCommand = GL_LINE_LOOP;
		data.mType = DebugShape::DEBUGQUAD;
		data.mColor = color;
		RenderManager::Instance().mDebugRenderData.push_back(data);
	}

	void DebugDrawManager::DebugAxes(glm::vec3 position, glm::vec3 scale, glm::vec3 color)
	{
		glm::mat4 model = glm::mat4(1.0f);
		model = glm::translate(model, position);
		model = glm::scale(model, scale);
		DebugRenderData data;
		data.mpMeshes.push_back(ResourceManager::Instance().GetShape(Shapes::AXES));
		data.mpModel = model;
		data.mDrawCommand = GL_LINES;
		data.mType = DebugShape::DEBUGAXES;
		data.mColor = color;
		RenderManager::Instance().mDebugRenderData.push_back(data);
	}

	void DebugDrawManager::DebugSphere(glm::vec3 position, glm::vec3 scale, glm::vec3 color)
	{
		glm::mat4 model = glm::mat4(1.0f);
		model = glm::translate(model, position);
		model = glm::scale(model, glm::vec3(scale));
		DebugRenderData data;
		data.mpMeshes.push_back(ResourceManager::Instance().GetShape(Shapes::SPHERE));
		data.mpModel = model;
		data.mDrawCommand = GL_LINES;
		data.mType = DebugShape::DEBUGSPHERE;
		data.mColor = color;
		RenderManager::Instance().mDebugRenderData.push_back(data);
	}

	void DebugDrawManager::DebugCube(glm::vec3 position, glm::vec3 scale, glm::vec3 color)
	{
		glm::mat4 model = glm::mat4(1.0f);
		model = glm::translate(model, position);
		model = glm::scale(model, scale);
		DebugRenderData data;
		data.mpMeshes.push_back(ResourceManager::Instance().GetShape(Shapes::CUBE));
		data.mpModel = model;
		data.mDrawCommand = GL_LINE_LOOP;
		data.mType = DebugShape::DEBUGCUBE;
		data.mColor = color;
		RenderManager::Instance().mDebugRenderData.push_back(data);
	}
}