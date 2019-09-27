#include <hollowpch.h>
#include "Mesh.h"
#include "VertexArray.h"
#include "VertexBuffer.h"
#include "ElementArrayBuffer.h"
#include "Shader.h"
#include "Utils/GLCall.h"

namespace Hollow
{
	Mesh::Mesh()
	{
	}

	Mesh::Mesh(const std::vector<Vertex>& vertices, const std::vector<unsigned int>& indices, const std::vector<MeshTexture>& textures)
		: mVertices(vertices), mIndices(indices), mTextures(textures)
	{
		InitMesh();
	}

	Mesh::~Mesh()
	{
		delete mpVAO;
		delete mpVBO;
		delete mpEBO;
	}


	void Mesh::Draw(Shader* pShader)
	{
		pShader->Use();
		mpVAO->Bind();
		GLCall(glDrawElements(GL_TRIANGLES, mpEBO->GetCount(), GL_UNSIGNED_INT, 0));
	}

	void Mesh::InitMesh()
	{
		mpVAO = new VertexArray();
		mpEBO = new ElementArrayBuffer();
		mpVBO = new VertexBuffer();

		// Bind VAO
		mpVAO->Bind();

		// Send vertex information to VBO
		mpVBO->AddData(&mVertices[0], mVertices.size() * sizeof(Vertex));

		// Set up index buffer EBO
		mpEBO->AddData(&mIndices[0], mIndices.size(), sizeof(unsigned int));

		// Position
		mpVAO->Push(3, GL_FLOAT, sizeof(float));
		// Normal
		mpVAO->Push(3, GL_FLOAT, sizeof(float));
		// Texture coordinates
		mpVAO->Push(2, GL_FLOAT, sizeof(float));
		// Tangent
		//mVAO.Push(3, GL_FLOAT, sizeof(float));
		mpVAO->AddLayout();
		// Unbind VAO
		mpVAO->Unbind();
	}
}