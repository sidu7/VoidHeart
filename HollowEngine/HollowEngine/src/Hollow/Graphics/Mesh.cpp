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
		delete mpVBO;
	}

	void Mesh::Draw(Shader* pShader)
	{
		pShader->Use();
		mpVAO->Bind();
		mpEBO->Bind();
		mpVBO->Bind();
		GLCall(glDrawElements(GL_TRIANGLES, mpEBO->GetCount(), GL_UNSIGNED_INT, 0));
		mpEBO->Unbind();
		mpVBO->Unbind();
		mpVAO->Unbind();
	}
}