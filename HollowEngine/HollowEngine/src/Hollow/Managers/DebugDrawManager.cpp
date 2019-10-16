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
		/*std::vector<glm::vec3> vertices;
		vertices.push_back(startPos);
		vertices.push_back(endPos);
		Mesh* mesh = new Mesh();
		mesh->mpVBO = new VertexBuffer();
		mesh->mpVBO->AddData(&vertices[0], 2 , sizeof(glm::vec3));
		mesh->mpVAO = new VertexArray();
		mesh->mpVAO->Bind();
		mesh->mpVAO->Push(3, GL_FLOAT, sizeof(float));
		mesh->mpVAO->AddLayout();
		mesh->mpVAO->Unbind();
		DebugRenderData data;
		data.mType = DebugShape::DEBUGLINE;
		data.mDrawCommand = GL_LINES;
		data.mpMeshes.push_back(mesh);
		data.mpModel = glm::mat4(1.0f);
		data.mColor = color;
		RenderManager::Instance().mDebugRenderData.push_back(data);*/
		glm::vec3 dirvec = endPos - startPos;
		glm::mat4 model = glm::mat4(1.0f);
		model = glm::translate(model, startPos);
		model *= glm::lookAt(startPos, endPos, glm::vec3(0.0f, 1.0f, 0.0f));
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