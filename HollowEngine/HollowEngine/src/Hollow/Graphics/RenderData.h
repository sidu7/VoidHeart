#pragma once
#include "glm/glm.hpp"

namespace Hollow {	
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
}