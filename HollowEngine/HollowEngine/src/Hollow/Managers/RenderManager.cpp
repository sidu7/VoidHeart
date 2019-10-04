// TODO: REMOVE THIS, TESTING PULL REQUEST
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
#include "Hollow/Graphics/FrameBuffer.h"

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

		// Initialize G-Buffer
		InitializeGBuffer();
	}

	void RenderManager::CleanUp()
	{
		delete mpCamera;

		delete mpDeferredShader;

		// CleanUp GBuffer things
		delete mpGBufferShader;
		delete mpGBuffer;
	}

	void RenderManager::Update()
	{
		// Initialize transform matrices
		mProjectionMatrix = glm::perspective(mpCamera->GetZoom(), (float)mpWindow->GetWidth() / mpWindow->GetHeight(), 0.1f, 1000.0f);
		mViewMatrix = mpCamera->GetViewMatrix();
		
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);	

		// Deferred G-Buffer Pass
		GBufferPass();

		// Apply global lighting
		GlobalLightingPass();

		mRenderData.clear();

		// Update ImGui
		DebugDisplay();
		ImGuiManager::Instance().Update();

		SDL_GL_SwapWindow(mpWindow->GetWindow());
	}

	void RenderManager::InitializeGBuffer()
	{
		// Compile G-Buffer shader and deferred shader
		mpGBufferShader = new Shader("Resources/Shaders/GBuffer.vert", "Resources/Shaders/GBuffer.frag");
		CreateDeferredShader();

		// Create G-Buffer
		mpGBuffer = new FrameBuffer(mpWindow->GetWidth(), mpWindow->GetHeight(), 4, true);

		// Set display mode
		mGBufferDisplayMode = 0;
	}

	void RenderManager::CreateDeferredShader()
	{
		mpDeferredShader = new Shader("Resources/Shaders/Deferred.vert", "Resources/Shaders/Deferred.frag");
		mpDeferredShader->Use();
		mpDeferredShader->SetInt("gPosition", 0);
		mpDeferredShader->SetInt("gNormal", 1);
		mpDeferredShader->SetInt("gDiffuse", 2);
		mpDeferredShader->SetInt("gSpecular", 3);
	}

	void RenderManager::GBufferPass()
	{
		glEnable(GL_DEPTH_TEST);
		mpGBuffer->Bind();
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		mpGBufferShader->Use();

		// Send view and projection matrix
		mpGBufferShader->SetMat4("View", mViewMatrix);
		mpGBufferShader->SetMat4("Projection", mProjectionMatrix);

		// Draw all game objects
		DrawAllRenderData(mpGBufferShader);

		mpGBuffer->Unbind();
	}

	void RenderManager::GlobalLightingPass()
	{
		// Clear opengl
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// Bind G-Buffer textures
		mpGBuffer->TexBind(0, 0);
		mpGBuffer->TexBind(1, 1);
		mpGBuffer->TexBind(2, 2);
		mpGBuffer->TexBind(3, 3);
		
		glDisable(GL_DEPTH_TEST);
		mpDeferredShader->Use();

		// Send light and view position
		mpDeferredShader->SetVec3("viewPosition", mpCamera->GetPosition());
		//mpDeferredShader->SetVec3("lightPosition", light->position);
		mpDeferredShader->SetVec3("lightPosition", glm::vec3(0.0f, 0.0f, 0.0f));

		// Send debug information
		mpDeferredShader->SetInt("displayMode", mGBufferDisplayMode);

		// Render FSQ
		DrawFSQ();
	}

	void RenderManager::DrawAllRenderData(Shader* pShader)
	{
		for (RenderData& data : mRenderData)
		{
			pShader->SetMat4("Model", data.mpModel);

			Material* pMaterial = data.mpMaterial;

			// Send lighting information
			pShader->SetVec3("diffuseColor", pMaterial->mDiffuseColor);

			// Send diffuse texture information
			pMaterial->mpTexture->Bind(1);
			pShader->SetInt("diffuseTexture", 1);

			pShader->SetVec3("specularColor", pMaterial->mSpecularColor);
			pShader->SetFloat("shininess", pMaterial->mShininess);

			// Draw object
			for (Mesh* mesh : data.mpMeshes)
			{
				mesh->Draw(pShader);
			}

			pMaterial->mpTexture->Unbind();
		}
	}
	void RenderManager::DrawFSQ()
	{
		static unsigned int quadVAO = 0;
		static unsigned int quadVBO;
		if (quadVAO == 0)
		{
			float quadVert[] = {
				-1.0f, 1.0f, 0.0f, 0.0f, 1.0f,
				-1.0f, -1.0f, 0.0f, 0.0f, 0.0f,
				1.0f, 1.0f, 0.0f, 1.0f, 1.0f,
				1.0f, -1.0f, 0.0f, 1.0f, 0.0f
			};

			// Setup plane vao
			glGenVertexArrays(1, &quadVAO);
			glGenBuffers(1, &quadVBO);
			glBindVertexArray(quadVAO);
			glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
			glBufferData(GL_ARRAY_BUFFER, sizeof(quadVert), &quadVert, GL_STATIC_DRAW);
			glEnableVertexAttribArray(0);
			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
			glEnableVertexAttribArray(1);
			glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
		}
		glBindVertexArray(quadVAO);
		glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
		glBindVertexArray(0);
	}
	void RenderManager::DebugDisplay()
	{
		if(ImGui::Begin("Renderer"))
		{
			DebugDisplayGBuffer();
		}
		ImGui::End();
	}
	void RenderManager::DebugDisplayGBuffer()
	{
		if (ImGui::CollapsingHeader("GBuffer"))
		{
			ImGui::InputInt("Display Mode", &mGBufferDisplayMode);
		}
	}
}