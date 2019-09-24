#include <hollowpch.h>
#include "RenderManager.h"

#include "ImGuiManager.h"

#include "Hollow/Components/Shape.h"
#include "Hollow/Components/Material.h"

#include "Hollow/Graphics/GameWindow.h"
#include "Hollow/Graphics/Camera.h"
#include "Hollow/Graphics/Shader.h"
#include "Hollow/Graphics/VertexBuffer.h"
#include "Hollow/Graphics/VertexArray.h"

namespace Hollow {

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
		//mpTestShader = new Shader("Resources/Shaders/Diffuse.vert", "Resources/Shaders/Diffuse.frag");
		mpTestShader = new Shader("Resources/Shaders/Phong.vert", "Resources/Shaders/Phong.frag");

		// Initialize transform matrices
		mProjectionMatrix = glm::perspective(mpCamera->GetZoom(), (float)mpWindow->GetWidth() / mpWindow->GetHeight(), 0.1f, 1000.0f);
		mViewMatrix = mpCamera->GetViewMatrix();
	}

	void RenderManager::CleanUp()
	{
		delete mpTestShader;
		delete mpCamera;
	}

	void RenderManager::Update()
	{
		glClearColor(0.0f, 1.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);	

		// TESTING BELOW ----------------------------
		// Draw some stuff
		glEnable(GL_DEPTH_TEST);
		mpTestShader->Use();
		mpTestShader->SetVec3("viewPosition", mpCamera->GetPosition());

		// Send MVP matrix
		mpTestShader->SetMat4("View", mViewMatrix);
		mpTestShader->SetMat4("Projection", mProjectionMatrix);
		//mpTestShader->SetMat4("Model", glm::mat4(0.1f));
		mpTestShader->SetMat4("Model", glm::rotate(glm::mat4(0.5f), glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f)));

		// TODO: Send actual lighting information
		//mpTestShader->SetVec3();
		mpTestShader->SetVec3("lightPosition", glm::vec3(0.0f, 10.0f, 0.0f));
		mpTestShader->SetVec3("lightColor", glm::vec3(1.0f, 1.0f, 1.0f));

		for (unsigned int objectIndex = 0; objectIndex < mShapes.size(); ++objectIndex)
		{
			Shape* pShape = mShapes[objectIndex];
			Material* pMaterial = mMaterials[objectIndex];

			// Send lighting information
			mpTestShader->SetVec3("diffuseColor", pMaterial->mDiffuseColor);
			mpTestShader->SetVec3("specularColor", pMaterial->mSpecularColor);
			mpTestShader->SetFloat("shininess", pMaterial->mShininess);

			// Draw object
			pShape->mpVAO->Bind();
			glDrawElements(GL_TRIANGLES, pShape->mIndices.size(), GL_UNSIGNED_INT, 0);
		}

		// Update ImGui
		ImGuiManager::Instance().Update();

		SDL_GL_SwapWindow(mpWindow->GetWindow());
	}
}