#include <hollowpch.h>
#include "RenderManager.h"

#include "ImGuiManager.h"

#include "Hollow/Components/Material.h"

#include "Hollow/Graphics/GameWindow.h"
#include "Hollow/Graphics/Shader.h"
#include "Hollow/Graphics/VertexBuffer.h"
#include "Hollow/Graphics/VertexArray.h"
#include "Hollow/Graphics/ElementArrayBuffer.h"
#include "Hollow/Graphics/Mesh.h"
#include "Hollow/Graphics/Texture.h"
#include "Hollow/Graphics/FrameBuffer.h"
#include "Hollow/Graphics/ShaderStorageBuffer.h"

#include "Hollow/Managers/FrameRateController.h"

#include "Utils/GLCall.h"

namespace Hollow {

	void RenderManager::Init(GameWindow* pWindow)
	{
		// Init opengl
		if (glewInit() != GLEW_OK)
		{
			HW_CORE_ERROR("Failed to initialize GLEW");
		}

		if (!glewIsSupported("GL_VERSION_4_3"))
		{
			HW_CORE_ERROR("Opengl 4.3 is not supported");
		}

		int mj, mn;
		glGetIntegerv(GL_MAJOR_VERSION, &mj);
		glGetIntegerv(GL_MINOR_VERSION, &mn);
		HW_CORE_INFO("Opengl version {0}.{1}", mj, mn);

		mpWindow = pWindow;
		
		// Initialize G-Buffer
		InitializeGBuffer();

		// Init ShadowMap shader
		mpShadowMapShader = new Shader("Resources/Shaders/ShadowMap.vert", "Resources/Shaders/ShadowMap.frag");

		// Init Debug Shader
		mpDebugShader = new Shader("Resources/Shaders/Debug.vert", "Resources/Shaders/Debug.frag");

		// Init Particle Shader
		mpParticleShader = new Shader("Resources/Shaders/ParticleSystem.vert", "Resources/Shaders/ParticleSystem.frag");
		mpParticleCompute = new Shader("Resources/Shaders/ParticleSystem.compute");
		GLCall(glEnable(GL_PROGRAM_POINT_SIZE));
	}

	void RenderManager::CleanUp()
	{

		delete mpDeferredShader;

		// CleanUp GBuffer things
		delete mpGBufferShader;
		delete mpGBuffer;
	}

	void RenderManager::Update()
	{
		GLCall(glClearColor(0.0f, 0.0f, 0.0f, 1.0f));
		GLCall(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));

		for (unsigned int i = 0; i < mLightData.size(); ++i)
		{	
			// ShadowMap Pass
			CreateShadowMap(mLightData[i]);
		}

		for (unsigned int i = 0; i < mCameraData.size(); ++i)
		{
			// Initialize transform matrices
			CameraData& camera = mCameraData[i];
			if (camera.mProjection == CameraProjection::PERSPECTIVE)
			{
				mProjectionMatrix = glm::perspective(camera.mZoom, (float)mpWindow->GetWidth() / mpWindow->GetHeight(), camera.mNearPlane, camera.mFarPlane);
			}
			else if (camera.mProjection == CameraProjection::ORTHOGRAPHIC)
			{
				//mProjectionMatrix = glm::ortho(0, mpWindow->GetWidth(), 0, mpWindow->GetHeight());
				mProjectionMatrix = glm::perspective(camera.mZoom, camera.mScreenViewPort.x / (float)camera.mScreenViewPort.y, camera.mNearPlane, camera.mFarPlane);
			}
			mViewMatrix = camera.mViewMatrix;

			if (camera.mType != CameraType::MAIN_CAMERA)
			{
				GLCall(glViewport(camera.mScreenPosition.x, camera.mScreenPosition.y, camera.mScreenViewPort.x, camera.mScreenViewPort.y));
			}
			else
			{
				GLCall(glViewport(0, 0, mpWindow->GetWidth(), mpWindow->GetHeight()));
			}			

			// Deferred G-Buffer Pass
			GBufferPass();
			for (unsigned int i = 0; i < mLightData.size(); ++i)
			{
				// Apply global lighting
				GlobalLightingPass(mLightData[i]);
			}

			if (ShowParticles)
			{
				DrawParticles();
			}

			if (camera.mType == CameraType::MAIN_CAMERA)
			{
				//Draw debug drawings
				DrawDebugDrawings();
			}
		}
		mParticleData.clear();
		mLightData.clear();
		mRenderData.clear();
		mCameraData.clear();
			   
		GLCall(glViewport(0, 0, mpWindow->GetWidth(), mpWindow->GetHeight()));

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

	void RenderManager::CreateShadowMap(LightData& light)
	{
		if (!light.mCastShadow)
		{
			return;
		}

		light.mpShadowMap->Bind();
		mpShadowMapShader->Use();

		glm::mat4 LightLookAt, LightProj;
		LightLookAt = glm::lookAt(light.mPosition, glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		LightProj = glm::perspective(glm::radians(45.0f), 1.0f, 0.1f, 2000.0f);

		mpShadowMapShader->SetMat4("Projection", LightProj);
		mpShadowMapShader->SetMat4("View", LightLookAt);

		light.mShadowMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(0.5f)) * glm::scale(glm::mat4(1.0f), glm::vec3(0.5f)) * LightProj * LightLookAt;

		glEnable(GL_CULL_FACE);
		glCullFace(GL_FRONT);

		DrawShadowCastingObjects(mpShadowMapShader);

		glDisable(GL_CULL_FACE);

		light.mpShadowMap->Unbind();
	}

	void RenderManager::GBufferPass()
	{
		GLCall(glEnable(GL_DEPTH_TEST));
		mpGBuffer->Bind();
		//glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		mpGBufferShader->Use();

		// Send view and projection matrix
		mpGBufferShader->SetMat4("View", mViewMatrix);
		mpGBufferShader->SetMat4("Projection", mProjectionMatrix);

		// Draw all game objects
		DrawAllRenderData(mpGBufferShader);

		mpGBufferShader->Unbind();
		mpGBuffer->Unbind();
	}

	void RenderManager::GlobalLightingPass(LightData& light)
	{
		// Clear opengl
		//glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// Bind G-Buffer textures
		mpGBuffer->TexBind(0, 0);
		mpGBuffer->TexBind(1, 1);
		mpGBuffer->TexBind(2, 2);
		mpGBuffer->TexBind(3, 3);
		
		glDisable(GL_DEPTH_TEST);
		mpDeferredShader->Use();

		// Send light and view position
		mpDeferredShader->SetVec3("viewPosition", mCameraData[0].mPosition);
		mpDeferredShader->SetVec3("lightPosition", light.mPosition);

		// Send ShadowMap texture and shadow matrix
		light.mpShadowMap->TexBind(0, 4);
		mpDeferredShader->SetInt("shadowMap", 4);
		mpDeferredShader->SetMat4("shadowMatrix", light.mShadowMatrix);

		// Send debug information
		mpDeferredShader->SetInt("displayMode", mGBufferDisplayMode);

		// Render FSQ
		DrawFSQ();

		mpGBuffer->TexUnbind(0);
		mpGBuffer->TexUnbind(1);
		mpGBuffer->TexUnbind(2);
		mpGBuffer->TexUnbind(3);
		light.mpShadowMap->TexUnbind(4);
	}

	void RenderManager::DrawAllRenderData(Shader* pShader)
	{
		for (RenderData& data : mRenderData)
		{
			pShader->SetMat4("Model", data.mpModel);
			pShader->SetMat4("NormalTr", glm::transpose(glm::inverse(data.mpModel)));

			pShader->SetInt("isAnimated", data.mIsAnimated);
			if (data.mIsAnimated)
			{
				for (unsigned int i = 0; i < data.mBoneTransforms.size(); ++i)
				{
					pShader->SetMat4("BoneTransforms[" + std::to_string(i) + "]", data.mBoneTransforms[i]);
				}
			}

			Material* pMaterial = data.mpMaterial;

			// Send lighting information
			pShader->SetVec3("diffuseColor", pMaterial->mDiffuseColor);

			// Send diffuse texture information
			if (pMaterial->mpTexture)
			{
				pMaterial->mpTexture->Bind(1);
				pShader->SetInt("diffuseTexture", 1);
			}

			pShader->SetVec3("specularColor", pMaterial->mSpecularColor);
			pShader->SetFloat("shininess", pMaterial->mShininess);

			// Draw object
			for (Mesh* mesh : data.mpMeshes)
			{
				if (mesh->mMaterialIndex != -1)
				{
					MaterialData* materialdata = pMaterial->mMaterials[mesh->mMaterialIndex];
					if (materialdata->mpDiffuse)
					{
						materialdata->mpDiffuse->Bind(1);
						pShader->SetInt("diffuseTexture", 1);
					}
					if (materialdata->mpSpecular)
					{
						materialdata->mpSpecular->Bind(2);
						pShader->SetInt("specularTexture", 2);
					}
					if (materialdata->mpNormal)
					{
						materialdata->mpNormal->Bind(3);
						pShader->SetInt("normalTexture", 3);
					}
					if (materialdata->mpHeight)
					{
						materialdata->mpHeight->Bind(4);
						pShader->SetInt("heightTexture", 4);
					}
				}
				mesh->mpVAO->Bind();
				mesh->mpEBO->Bind();
				mesh->mpVBO->Bind();
				GLCall(glDrawElements(GL_TRIANGLES, mesh->mpEBO->GetCount(), GL_UNSIGNED_INT, 0));
				mesh->mpEBO->Unbind();
				mesh->mpVBO->Unbind();
				mesh->mpVAO->Unbind();
				if (mesh->mMaterialIndex != -1)
				{
					MaterialData* materialdata = pMaterial->mMaterials[mesh->mMaterialIndex];
					if (materialdata->mpDiffuse)
					{
						materialdata->mpDiffuse->Unbind(1);
					}
					if (materialdata->mpSpecular)
					{
						materialdata->mpSpecular->Unbind(2);
					}
					if (materialdata->mpNormal)
					{
						materialdata->mpNormal->Unbind(3);
					}
					if (materialdata->mpHeight)
					{
						materialdata->mpHeight->Unbind(4);
					}
				}
			}

			if (pMaterial->mpTexture)
			{
				pMaterial->mpTexture->Unbind();
			}
		}
	}

	void RenderManager::DrawShadowCastingObjects(Shader* pShader)
	{
		GLCall(glEnable(GL_CULL_FACE));
		GLCall(glCullFace(GL_FRONT));
		for (RenderData& data : mRenderData)
		{
			if (!data.mCastShadow)
			{
				continue;
			}

			pShader->SetMat4("Model", data.mpModel);

			pShader->SetInt("isAnimated", data.mIsAnimated);
			if (data.mIsAnimated)
			{
				for (unsigned int i = 0; i < data.mBoneTransforms.size(); ++i)
				{
					pShader->SetMat4("BoneTransforms[" + std::to_string(i) + "]", data.mBoneTransforms[i]);
				}
			}

			// Draw object
			for (Mesh* mesh : data.mpMeshes)
			{
				mesh->mpVAO->Bind();
				mesh->mpEBO->Bind();
				mesh->mpVBO->Bind();
				GLCall(glDrawElements(GL_TRIANGLES, mesh->mpEBO->GetCount(), GL_UNSIGNED_INT, 0));
				mesh->mpEBO->Unbind();
				mesh->mpVBO->Unbind();
				mesh->mpVAO->Unbind();
			}
		}
		GLCall(glDisable(GL_CULL_FACE));
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

	void RenderManager::DrawParticles()
	{
		// Draw Particles 
		mpParticleShader->Use();
		mpParticleShader->SetMat4("View", mViewMatrix);
		mpParticleShader->SetMat4("Projection", mProjectionMatrix);

		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		for (unsigned int i = 0; i < mParticleData.size(); ++i)
		{
			ParticleData& particle = mParticleData[i];
			mpParticleShader->SetInt("Type", particle.mType);
			if (particle.mType == POINT)
			{
				//Compute particle positions according to velocities
				particle.mpShaderStorage->Bind(2);

				mpParticleCompute->Use();
				mpParticleCompute->SetVec3("Center", particle.mCenter);
				mpParticleCompute->SetFloat("DeltaTime", FrameRateController::Instance().GetFrameTime());
				mpParticleCompute->DispatchCompute(particle.mParticlesCount / 128, 1, 1);
				particle.mpShaderStorage->PutMemoryBarrier();
				
				mpParticleShader->Use();
				mpParticleShader->SetMat4("Model", particle.mModel);
				mpParticleShader->SetVec2("ScreenSize", glm::vec2(mpWindow->GetWidth(), mpWindow->GetHeight()));
				mpParticleShader->SetFloat("SpriteSize", 0.1f);
								
				particle.mTex->Bind(4);
				mpParticleShader->SetInt("Texx", 4);
				particle.mpParticleVAO->Bind();
				
				GLCall(glDrawArrays(GL_POINTS, 0, particle.mParticlesCount));
				particle.mTex->Unbind(4);
				particle.mpShaderStorage->Unbind();
				particle.mpParticleVAO->Unbind();
			}
			else if(particle.mType == MODEL)
			{
				particle.mpParticleVBO->AddSubData(
					&particle.mParticleModelMatrices[0], // data
					particle.mParticleModelMatrices.size(), sizeof(glm::mat4)); // size of data to be sent

				for (Mesh* mesh : particle.mParticleModel)
				{
					mesh->mpVAO->Bind();
					mesh->mpVBO->Bind();
					mesh->mpEBO->Bind();
					particle.mpParticleVBO->Bind();
					glDrawElementsInstanced(GL_TRIANGLES, mesh->mpEBO->GetCount(), GL_UNSIGNED_INT, 0, particle.mParticlesCount);
					particle.mpParticleVBO->Unbind();
					mesh->mpEBO->Unbind();
					mesh->mpVBO->Unbind();
					mesh->mpVAO->Unbind();
				}
			}
		}
		glDisable(GL_BLEND);		
	}

	void RenderManager::DrawDebugDrawings()
	{
		mpDebugShader->Use();
		mpDebugShader->SetMat4("View", mViewMatrix);
		mpDebugShader->SetMat4("Projection", mProjectionMatrix);

		for (unsigned int i = 0; i < mDebugRenderData.size(); ++i)
		{
			DebugRenderData& data = mDebugRenderData[i];
			mpDebugShader->SetMat4("Model", data.mpModel);
			mpDebugShader->SetVec3("Color", data.mColor);
			for (Mesh* mesh : data.mpMeshes)
			{
				mesh->mpVAO->Bind();
				mesh->mpVBO->Bind();
				if (mesh->mpEBO)
				{
					mesh->mpEBO->Bind();
					GLCall(glDrawElements(data.mDrawCommand, mesh->mpEBO->GetCount(), GL_UNSIGNED_INT, 0));
					mesh->mpEBO->Unbind();
				}
				else
				{
					GLCall(glDrawArrays(data.mDrawCommand, 0, mesh->mpVBO->GetVerticesCount()));
				}
				mesh->mpVAO->Unbind();
				mesh->mpVBO->Unbind();
			}
		}

		mDebugRenderData.clear();
	}

	void RenderManager::DebugDisplay()
	{
		if(ImGui::Begin("Renderer"))
		{
			DebugDisplayGBuffer();
			ImGui::Checkbox("Particle System",&ShowParticles);
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