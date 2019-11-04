#pragma once

namespace Hollow
{
	class Texture;
	class VertexArray;
	class VertexBuffer;
	class ElementArrayBuffer;
	
	struct UIRenderData
	{
		glm::vec3 mPosition;
		Texture* mpTexture;
		VertexArray* mpVAO;
		VertexBuffer* mpVBO;
		ElementArrayBuffer* mpEBO;
	};
}