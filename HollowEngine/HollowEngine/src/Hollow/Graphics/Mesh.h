#pragma once
#include "Hollow/Common.h"

namespace Hollow
{
	class Shader;
	class VertexArray;
	class ElementArrayBuffer;
	class VertexBuffer;

	class Mesh
	{
	public:
		/// <summary>
		/// Initializes a new instance of the <see cref="Mesh"/> class.
		/// </summary>
		HOLLOW_API Mesh();

		/// <summary>
		/// Destructor for an instance of the <see cref="Mesh"/> class.
		/// </summary>
		HOLLOW_API ~Mesh();

		VertexArray* mpVAO;
		ElementArrayBuffer* mpEBO;
		VertexBuffer* mpVBO;
		unsigned int mMaterialIndex;
	};
}