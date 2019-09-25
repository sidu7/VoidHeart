#pragma once
#include "glm/glm.hpp"

namespace Hollow {
	class VertexArray;
	class ElementArrayBuffer;
	class Shader;
	class Material;

	struct RenderData
	{
		VertexArray* mpVAO;
		ElementArrayBuffer* mpEBO;
		glm::mat4 mpModel;
		Material* mpMaterial;
		Shader* mpShader;
	};
}