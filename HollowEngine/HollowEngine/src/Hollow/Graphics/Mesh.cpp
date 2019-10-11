#include <hollowpch.h>
#include "Mesh.h"
#include "VertexArray.h"
#include "VertexBuffer.h"
#include "ElementArrayBuffer.h"
#include "Shader.h"
#include "Utils/GLCall.h"

namespace Hollow
{
	Mesh::Mesh() : mpVAO(nullptr), mpEBO(nullptr), mpVBO(nullptr)
	{
	}

	Mesh::~Mesh()
	{
		delete mpVAO;
		delete mpEBO;
	}

	void Mesh::Draw(Shader* pShader)
	{
		pShader->Use();
		mpVAO->Bind();
		GLCall(glDrawElements(GL_TRIANGLES, mpEBO->GetCount(), GL_UNSIGNED_INT, 0));
	}
}