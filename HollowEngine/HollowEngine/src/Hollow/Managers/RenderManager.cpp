#include <hollowpch.h>
#include "RenderManager.h"

#include "Hollow/Graphics/GameWindow.h"
#include "Hollow/Graphics/Camera.h"
#include "Hollow/Graphics/Shader.h"

namespace Hollow {

	RenderManager::~RenderManager()
	{
		delete mpTestShader;
		delete mpCamera;
		
	}

	void RenderManager::Init(GameWindow* pWindow)
	{
		// Init opengl
		if (glewInit() != GLEW_OK)
		{
			HW_CORE_ERROR("Failed to initialize GLEW");
		}

		mpWindow = pWindow;
		mpCamera = new Camera();

		// TESTING BELOW ----------------------------
		// Create a shader
		mpTestShader = new Shader("Resources/Shaders/Diffuse.vert", "Resources/Shaders/Diffuse.frag");

		// Create VAO and VBO
		MakeVAO();

		// Initialize transform matrices
		mProjectionMatrix = glm::perspective(mpCamera->GetZoom(), 1.0f, 0.1f, 1000.0f);
		mViewMatrix = mpCamera->GetViewMatrix();
	}

	void RenderManager::Update()
	{
		glClearColor(0.0f, 1.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);	

		// TESTING BELOW ----------------------------
		// Draw some stuff
		glEnable(GL_DEPTH_TEST);
		mpTestShader->Use();
		mpTestShader->SetVec3("diffuseColor", 1.0f, 0.0f, 1.0f);

		// Send MVP matrix
		mpTestShader->SetMat4("View", mViewMatrix);
		mpTestShader->SetMat4("Projecion", mProjectionMatrix);
		mpTestShader->SetMat4("Model", glm::mat4(0.5f));

		glBindVertexArray(mVAO);
		glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, 0);

		SDL_GL_SwapWindow(mpWindow->GetWindow());
	}

	void RenderManager::MakeVAO()
	{
		float vertices[] = { 1.0, 0.0, 0.0,
						0.0, 1.0, 0.0,
						-1.0, 0.0, 0.0 };
		unsigned int indices[] = { 0, 1, 2 };

		// Generate vertex array
		glGenVertexArrays(1, &mVAO);
		glGenBuffers(1, &mVBO);
		glGenBuffers(1, &mEBO);

		// Set up VAO
		glBindVertexArray(mVAO);

		// Send vertex information to VBO
		glBindBuffer(GL_ARRAY_BUFFER, mVBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
		//glBufferData(GL_ARRAY_BUFFER, mVertices.size() * sizeof(Vertex), &mVertices[0], GL_STATIC_DRAW);

		// Set up index buffer EBO
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mEBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
		//glBufferData(GL_ELEMENT_ARRAY_BUFFER, mIndices.size() * sizeof(unsigned int), &mIndices[0], GL_STATIC_DRAW);

		// Position
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
		//glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
		// Normal
		//glEnableVertexAttribArray(1);
		//glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(3 * sizeof(float)));
		// Texture coordinates
		//glEnableVertexAttribArray(2);
		//glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(6 * sizeof(float)));
		// Tangent
		//glEnableVertexAttribArray(3);
		//glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(8 * sizeof(float)));

		// Unbind VAO
		glBindVertexArray(0);
	}
}