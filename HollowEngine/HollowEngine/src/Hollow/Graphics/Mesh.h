#pragma once

namespace Hollow
{
	class Shader;
	class VertexArray;
	class ElementArrayBuffer;
	class VertexBuffer;

	class Mesh
	{
	public:
		HOLLOW_API Mesh();
		HOLLOW_API ~Mesh();

		VertexArray* mpVAO;
		ElementArrayBuffer* mpEBO;
		VertexBuffer* mpVBO;
		unsigned int mMaterialIndex;
	};
}