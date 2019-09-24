#pragma once
#include "glm/glm.hpp"

namespace Hollow {
	class VertexArray;
	class ElementBufferArray;
	class Shader;

	class RenderData
	{
	public:
	private:
		VertexArray* mVAO;
		ElementBufferArray* mEBO;
		glm::mat4 mModel;
		Shader* shader;
		//glm::vec3 mDiffuseColor;
	};
}