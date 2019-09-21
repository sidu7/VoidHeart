#include <hollowpch.h>
#include "RenderManager.h"

#include "Hollow/Graphics/GameWindow.h"
#include "Hollow/Graphics/Camera.h"
#include "Hollow/Graphics/Shader.h"
#include "Hollow/Graphics/VertexBuffer.h"
#include "Hollow/Graphics/VertexArray.h"

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

		// Initialize transform matrices
		mProjectionMatrix = glm::perspective(mpCamera->GetZoom(), (float)mpWindow->GetWidth() / mpWindow->GetHeight(), 0.1f, 1000.0f);
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
		mpTestShader->SetVec3("viewPosition", mpCamera->GetPosition());

		// Send MVP matrix
		mpTestShader->SetMat4("View", mViewMatrix);
		mpTestShader->SetMat4("Projection", mProjectionMatrix);
		//mpTestShader->SetMat4("Model", glm::mat4(0.1f));
		mpTestShader->SetMat4("Model", glm::rotate(glm::mat4(0.5f), glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f)));

		for (Shape* pShape : mShapes)
		{
			pShape->mpVAO->Bind();
			glDrawElements(GL_TRIANGLES, pShape->mIndices.size(), GL_UNSIGNED_INT, 0);
		}

		SDL_GL_SwapWindow(mpWindow->GetWindow());
	}
}