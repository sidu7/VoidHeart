#pragma once

namespace Hollow
{
#ifndef VERTEX
#define VERTEX
	struct Vertex
	{
		glm::vec3 position;
		glm::vec3 normal;
		glm::vec2 tex;
		//glm::vec3 tangent;
	};
#endif

	class Shader;
	class VertexArray;
	class ElementArrayBuffer;
	class VertexBuffer;

	class Mesh
	{
	public:
		Mesh();
		~Mesh();

		VertexArray* mpVAO;
		ElementArrayBuffer* mpEBO;
		VertexBuffer* mpVBO;
		unsigned int mMaterialIndex;
	};
}