#include <hollowpch.h>
#include "Mesh.h"
#include "VertexArray.h"
#include "VertexBuffer.h"
#include "ElementArrayBuffer.h"
#include "Shader.h"

namespace Hollow
{
	Mesh::Mesh() : mpVAO(nullptr), mpEBO(nullptr), mpVBO(nullptr), mMaterialIndex(-1)
	{
	}

	Mesh::~Mesh()
	{
		delete mpVAO;
		delete mpEBO;
		delete mpVBO;
	}
}