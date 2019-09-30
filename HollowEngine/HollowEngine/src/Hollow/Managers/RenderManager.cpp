#include <hollowpch.h>
#include "RenderManager.h"

#include "ImGuiManager.h"

#include "Hollow/Components/Shape.h"
#include "Hollow/Components/Material.h"
#include "Hollow/Components/Transform.h"

#include "Hollow/Graphics/GameWindow.h"
#include "Hollow/Graphics/Camera.h"
#include "Hollow/Graphics/Shader.h"
#include "Hollow/Graphics/VertexBuffer.h"
#include "Hollow/Graphics/VertexArray.h"
#include "Hollow/Graphics/ElementArrayBuffer.h"
#include "Hollow/Graphics/Mesh.h"
#include "Hollow/Graphics/Texture.h"

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
		mpDebugShader = new Shader("Resources/Shaders/Debug.vert", "Resources/Shaders/Debug.frag");
	}

	void RenderManager::CleanUp()
	{
		delete mpTestShader;
		delete mpCamera;
	}

	void RenderManager::Update()
	{
		// Initialize transform matrices
		mProjectionMatrix = glm::perspective(mpCamera->GetZoom(), (float)mpWindow->GetWidth() / mpWindow->GetHeight(), 0.1f, 1000.0f);
		mViewMatrix = mpCamera->GetViewMatrix();
		
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

		// TODO: Send actual lighting information
		//mpTestShader->SetVec3();
		mpTestShader->SetVec3("lightPosition", glm::vec3(0.0f, 10.0f, 0.0f));
		mpTestShader->SetVec3("lightColor", glm::vec3(1.0f, 1.0f, 1.0f));

		for (unsigned int i = 0; i < mRenderData.size(); ++i)
		{
			RenderData& data = mRenderData[i];

			mpTestShader->SetMat4("Model", data.mpModel);
			
			Material* pMaterial = data.mpMaterial;
						
			//mpTestShader->SetMat4("Model", glm::mat4(0.1f));
			// Send lighting information
			mpTestShader->SetVec3("diffuseColor", pMaterial->mDiffuseColor);
			data.mpMaterial->mpTexture->Bind(1);
			mpTestShader->SetInt("texture_diffuse", 1);
			mpTestShader->SetVec3("specularColor", pMaterial->mSpecularColor);
			mpTestShader->SetFloat("shininess", pMaterial->mShininess);

			// Draw object
			for (Mesh* mesh : data.mpMeshes)
			{
				mesh->Draw(mpTestShader);
			}
			data.mpMaterial->mpTexture->Unbind();
		}

		mRenderData.clear();

		mpDebugShader->Use();
		mpDebugShader->SetMat4("View", mViewMatrix);
		mpDebugShader->SetMat4("Projection", mProjectionMatrix);

		for (unsigned int i = 0; i < mDebugRenderData.size(); ++i)
		{
			RenderData& data = mDebugRenderData[i];

			mpDebugShader->SetMat4("Model", data.mpModel);
			
			for (Mesh* mesh : data.mpMeshes)
			{
				mesh->mpVAO->Bind();
				glDrawElements(GL_LINE_LOOP, mesh->mpEBO->GetCount(), GL_UNSIGNED_INT, 0);
			}
		}

		mDebugRenderData.clear();

		// Update ImGui
		ImGuiManager::Instance().Update();

		SDL_GL_SwapWindow(mpWindow->GetWindow());
	}
}