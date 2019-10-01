#pragma once
#include "glm/glm.hpp"

namespace Hollow {
	class VertexArray;
	class ElementArrayBuffer;
	class Shader;
	class Material;
	class Mesh;

	struct RenderData
	{
		std::vector<Mesh*> mpMeshes;
		glm::mat4 mpModel;
		Material* mpMaterial;
		Shader* mpShader;
	};

	struct DebugRenderData
	{
		std::vector<Mesh*> mpMeshes;
		glm::mat4 mpModel;
		GLenum mDrawCommand;
	};
}