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

	void RenderManager::Init(rapidjson::Value::Object& data, GameWindow* pWindow)
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
		InitializeGBuffer(data);

		// Initialize local light shader
		CreateLocalLightShader();

		// Init ShadowMap shader
		mpShadowMapShader = new Shader(data["ShadowMapShader"].GetArray()[0].GetString(), data["ShadowMapShader"].GetArray()[1].GetString());
		mpShadowDebugShader = new Shader("Resources/Shaders/ShadowDebug.vert", "Resources/Shaders/ShadowDebug.frag");

		// Init blur shader
		mpHorizontalBlurShader = new Shader("Resources/Shaders/HorizontalBlur.comp");
		mpVerticalBlurShader = new Shader("Resources/Shaders/VerticalBlur.comp");

		// Init Debug Shader
		mpDebugShader = new Shader(data["DebugShader"].GetArray()[0].GetString(), data["DebugShader"].GetArray()[1].GetString());

		// Init Particle Shader
		mpParticleShader = new Shader(data["ParticleShader"].GetArray()[0].GetString(), data["ParticleShader"].GetArray()[1].GetString());
		mpParticleCompute = new Shader(data["ParticleCompute"].GetString());
		mpParticlesPositionStorage = new ShaderStorageBuffer();
		mpParticlesPositionStorage->CreateBuffer(MAX_PARTICLES_COUNT * sizeof(glm::vec4));
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
			// Blur ShadowMap
			BlurShadowMap(mLightData[i]);
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

		GLCall(glViewport(0, 0, mpWindow->GetWidth(), mpWindow->GetHeight()));

		// Local lighting pass
		LocalLightingPass();

		// Render ShadowMap as FSQ if a debug mode is set
		if (mShadowMapDebugMode > 0)
		{
			DrawShadowMap();
		}	   

		// Update ImGui
		DebugDisplay();
		ImGuiManager::Instance().Update();

		SDL_GL_SwapWindow(mpWindow->GetWindow());

		mParticleData.clear();
		mLightData.clear();
		mRenderData.clear();
		mCameraData.clear();
	}

	inline glm::vec2 RenderManager::GetWindowSize()
	{
		return glm::vec2(mpWindow->GetWidth(), mpWindow->GetHeight());
	}

	void RenderManager::InitializeGBuffer(rapidjson::Value::Object& data)
	{
		// Compile G-Buffer shader and deferred shader
		mpGBufferShader = new Shader(data["GBufferShader"].GetArray()[0].GetString(), data["GBufferShader"].GetArray()[1].GetString());
		CreateDeferredShader(data);

		// Create G-Buffer
		mpGBuffer = new FrameBuffer(mpWindow->GetWidth(), mpWindow->GetHeight(), 4, true);

		// Set display mode
		mGBufferDisplayMode = 0;
	}

	void RenderManager::CreateDeferredShader(rapidjson::Value::Object& data)
	{
		mpDeferredShader = new Shader(data["DeferredShader"].GetArray()[0].GetString(), data["DeferredShader"].GetArray()[1].GetString());
		mpDeferredShader->Use();
		mpDeferredShader->SetInt("gPosition", 0);
		mpDeferredShader->SetInt("gNormal", 1);
		mpDeferredShader->SetInt("gDiffuse", 2);
		mpDeferredShader->SetInt("gSpecular", 3);
	}

	void RenderManager::CreateLocalLightShader()
	{
		mpLocalLightShader = new Shader("Resources/Shaders/LocalLight.vert", "Resources/Shaders/LocalLight.frag");
		mpLocalLightShader->Use();
		mpLocalLightShader->SetInt("gPosition", 0);
		mpLocalLightShader->SetInt("gNormal", 1);
		mpLocalLightShader->SetInt("gDiffuse", 2);
		mpLocalLightShader->SetInt("gSpecular", 3);
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
		// Calculate light up vector
		//t0 = glm::normalize(glm::vec3(c->collisionNormal.y, -c->collisionNormal.x, 0.0f));
		LightLookAt = glm::lookAt(light.mPosition, glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		LightProj = glm::perspective(glm::radians(45.0f), (float)mpWindow->GetWidth() / (float)mpWindow->GetHeight(), light.mShadowMapNearPlane, light.mShadowMapFarPlane);

		mpShadowMapShader->SetMat4("Projection", LightProj);
		mpShadowMapShader->SetMat4("View", LightLookAt);

		//light.mShadowMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(0.5f)) * glm::scale(glm::mat4(1.0f), glm::vec3(0.5f)) * LightProj * LightLookAt;
		light.mShadowMatrix = LightProj * LightLookAt;

		//glEnable(GL_CULL_FACE);
		//glCullFace(GL_FRONT);

		DrawShadowCastingObjects(mpShadowMapShader);

		//glDisable(GL_CULL_FACE);

		light.mpShadowMap->Unbind();
	}

	void RenderManager::BlurShadowMap(LightData& light)
	{
		// Create blur kernel
		std::vector<float> weights = CreateBlurKernel(light.mBlurDistance);

		// Apply horizontal blur
		// Make horizontal blur shader current
		mpHorizontalBlurShader->Use();

		// Send block of weights to shader as uniform block
		GLuint hBlockID;
		GLuint hBindpoint = 0;
		// TODO: Refactor in to function
		glGenBuffers(1, &hBlockID);
		GLint loc = glGetUniformBlockIndex(mpHorizontalBlurShader->mProgram, "blurKernel");
		glUniformBlockBinding(mpHorizontalBlurShader->mProgram, loc, hBindpoint);
		glBindBufferBase(GL_UNIFORM_BUFFER, hBindpoint, hBlockID);
		glBufferData(GL_UNIFORM_BUFFER, weights.size() * sizeof(float), &weights[0], GL_STATIC_DRAW);

		// Send blur distance to shader
		mpHorizontalBlurShader->SetUInt("blurDistance", light.mBlurDistance);

		// Send texture to shader
		GLuint hImageUnit = 0;
		glBindImageTexture(hImageUnit, light.mpShadowMap->mpTextureID[0], 0, GL_FALSE, 0, GL_READ_ONLY, GL_RGBA32F);
		mpHorizontalBlurShader->SetInt("src", hImageUnit++);
		//Texture intermediate();
		static GLuint intermediate = 0;
		if (intermediate == 0)
		{
			glGenTextures(1, &intermediate);
			glBindTexture(GL_TEXTURE_2D, intermediate);
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, 2048, 2048, 0, GL_RGBA, GL_FLOAT, 0);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
			float borderColor[] = { 1.0f, 1.0f, 1.0f, 1.0f };
			glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);
		}

		glBindImageTexture(hImageUnit, intermediate, 0, GL_FALSE, 0, GL_WRITE_ONLY, GL_RGBA32F);
		mpHorizontalBlurShader->SetInt("dst", hImageUnit++);

		// Uniforms and image variables
		int W = 2048;
		int H = 2048;
		glDispatchCompute(W / 128, H, 1);
		glMemoryBarrier(GL_ALL_BARRIER_BITS);

		glUseProgram(0);

		// Apply vertical blur
		// Make vertical blur shader current
		mpVerticalBlurShader->Use();

		// Send block of weights to shader as uniform block
		GLuint blockID;
		GLuint bindpoint = 0;
		glGenBuffers(1, &blockID);
		loc = glGetUniformBlockIndex(mpVerticalBlurShader->mProgram, "blurKernel");
		glUniformBlockBinding(mpVerticalBlurShader->mProgram, loc, bindpoint);
		glBindBufferBase(GL_UNIFORM_BUFFER, bindpoint, blockID);
		glBufferData(GL_UNIFORM_BUFFER, weights.size() * sizeof(float), &weights[0], GL_STATIC_DRAW);

		// Send blur distance to shader
		mpVerticalBlurShader->SetUInt("blurDistance", light.mBlurDistance);

		// Send texture to shader
		GLuint imageUnit = 0;
		glBindImageTexture(imageUnit, intermediate, 0, GL_FALSE, 0, GL_READ_ONLY, GL_RGBA32F);
		mpVerticalBlurShader->SetInt("src", imageUnit++);
		glBindImageTexture(imageUnit, light.mpShadowMap->mpTextureID[0], 0, GL_FALSE, 0, GL_WRITE_ONLY, GL_RGBA32F);
		mpVerticalBlurShader->SetInt("dst", imageUnit++);

		// Uniforms and image variables
		glDispatchCompute(W, H / 128, 1);
		glMemoryBarrier(GL_ALL_BARRIER_BITS);

		glUseProgram(0);
	}

	std::vector<float> RenderManager::CreateBlurKernel(unsigned int distance)
	{
		// Set up resources it will need
		int w = distance;
		if (distance > 50)
		{
			w = 50;
		}
		float s = (float)w / 2.0f;
		std::vector<float> weights;
		float totalWeight = 0.0f;

		// Calculate weights
		for (int i = -w; i <= w; ++i)
		{
			float exponent = (-0.5f * i * i) / (s * s);
			float weight = std::expf(exponent);
			weights.push_back(weight);
			totalWeight += weight;
		}

		// Normalize weights
		for (auto& weight : weights)
		{
			weight /= totalWeight;
		}

		return weights;
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
		mpDeferredShader->SetFloat("alpha", light.mAlpha);
		mpDeferredShader->SetFloat("md", light.mMD);

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

	void RenderManager::LocalLightingPass()
	{
		// Bind G-Buffer textures
		mpGBuffer->TexBind(0, 0);
		mpGBuffer->TexBind(1, 1);
		mpGBuffer->TexBind(2, 2);
		mpGBuffer->TexBind(3, 3);

		//if (mDisplayMode == 0 && mLocalLighting)
		{
			// Enable blending, disable depth testing, turn face culling on
			glEnable(GL_BLEND);
			glBlendFunc(GL_ONE, GL_ONE);
			glDisable(GL_DEPTH_TEST);
			glEnable(GL_CULL_FACE);
			glCullFace(GL_FRONT);

			mpLocalLightShader->Use();

			// Send screen size to GPU
			mpLocalLightShader->SetVec2("screenSize", glm::vec2((float)mpWindow->GetWidth(), (float)mpWindow->GetHeight()));

			// Set projection and view matrix
			mpLocalLightShader->SetMat4("Projection", mProjectionMatrix);
			mpLocalLightShader->SetMat4("View", mViewMatrix);
			mpLocalLightShader->SetVec3("viewPosition", mCameraData[0].mPosition);

			// Draw light volume
			for (auto& light : mLightData)
			{
				mpLocalLightShader->SetFloat("lightRadius", light.mRadius);
				mpLocalLightShader->SetVec3("lightPosition", light.mPosition);

				// Get the light transform
				glm::mat4 lightTransform(1.0f);
				lightTransform = glm::translate(lightTransform, light.mPosition);
				lightTransform = glm::scale(lightTransform, glm::vec3(light.mRadius));

				mpLocalLightShader->SetMat4("Model", lightTransform);
				//DrawLight(light);
				DrawSphere();
			}
			glDisable(GL_BLEND);
			glDisable(GL_CULL_FACE);
			glEnable(GL_DEPTH_TEST);
		}

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
			pShader->SetVec3("specularColor", pMaterial->mSpecularColor);
			pShader->SetFloat("shininess", pMaterial->mShininess);

			// Send diffuse texture information
			if (pMaterial->mpTexture)
			{
				pMaterial->mpTexture->Bind(1);
				pShader->SetInt("diffuseTexture", 1);
			}

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
				pMaterial->mpTexture->Unbind(1);
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

	void RenderManager::DrawSphere()
	{
		// TODO: Overhaul this function
		static unsigned int sphereVAO = 0;
		static unsigned int sphereVBO;
		static unsigned int sphereEBO;
		if (sphereVAO == 0)
		{
			// Send light vertex information to VBO
			std::vector<float> lightVertices;
			std::vector<unsigned int> lightIndices;
			const float PI = 3.141592f;
			//Number of subdivisions
			int n = 32;
			float d = 2.0f * PI / float(n * 2);
			for (int i = 0; i <= n * 2; ++i)
			{
				float s = i * 2.0f * PI / float(n * 2);
				for (int j = 0; j <= n; ++j)
				{
					float t = j * PI / float(n);
					float x = cos(s) * sin(t);
					float y = sin(s) * sin(t);
					float z = cos(t);

					// Create vertex
					lightVertices.push_back(x);
					lightVertices.push_back(y);
					lightVertices.push_back(z);

					// Add indices
					if (i > 0 && j > 0)
					{
						// i, j, k
						lightIndices.push_back((i - 1) * (n + 1) + (j - 1));
						lightIndices.push_back((i - 1) * (n + 1) + j);
						lightIndices.push_back((i) * (n + 1) + j);
						// i, k, l
						lightIndices.push_back((i - 1) * (n + 1) + (j - 1));
						lightIndices.push_back((i) * (n + 1) + j);
						lightIndices.push_back((i) * (n + 1) + (j - 1));
					}
				}
			}
			// Generate sphere vertex array
			glGenVertexArrays(1, &sphereVAO);
			glGenBuffers(1, &sphereVBO);
			glGenBuffers(1, &sphereEBO);

			// Bind VAO
			glBindVertexArray(sphereVAO);

			// Send vertex information to VBO
			glBindBuffer(GL_ARRAY_BUFFER, sphereVBO);
			glBufferData(GL_ARRAY_BUFFER, lightVertices.size() * sizeof(float), &lightVertices[0], GL_STATIC_DRAW);

			// Set up index buffer EBO
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, sphereEBO);
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, lightIndices.size() * sizeof(unsigned int), &lightIndices[0], GL_STATIC_DRAW);

			// Position attribute for VAO
			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
			glEnableVertexAttribArray(0);
		}

		glBindVertexArray(sphereVAO);
		int size;
		glGetBufferParameteriv(GL_ELEMENT_ARRAY_BUFFER, GL_BUFFER_SIZE, &size);
		glDrawElements(GL_TRIANGLES, size / sizeof(unsigned int), GL_UNSIGNED_INT, 0);
		// Unbind VAO
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
				particle.mpParticleDataStorage->Bind(2);
				mpParticlesPositionStorage->Bind(3);

				mpParticleCompute->Use();
				mpParticleCompute->SetVec3("Center", particle.mCenter);
				mpParticleCompute->SetFloat("DeltaTime", FrameRateController::Instance().GetFrameTime());
				mpParticleCompute->SetVec2("SpeedRange", particle.mSpeedRange);
				mpParticleCompute->SetVec2("LifeRange", particle.mLifeRange);
				mpParticleCompute->DispatchCompute(particle.mParticlesCount / 128, 1, 1);
				ShaderStorageBuffer::PutMemoryBarrier();
				particle.mpParticleDataStorage->Unbind(2);	

				mpParticleShader->Use();
				mpParticleShader->SetMat4("Model", particle.mModel);
				mpParticleShader->SetVec2("ScreenSize", glm::vec2(mpWindow->GetWidth(), mpWindow->GetHeight()));
				mpParticleShader->SetFloat("SpriteSize", 0.1f);
								
				particle.mTex->Bind(4);
				mpParticleShader->SetInt("Texx", 4);
				particle.mpParticleVAO->Bind();
				
				GLCall(glDrawArrays(GL_POINTS, 0, particle.mParticlesCount));
				particle.mTex->Unbind(4);
				mpParticlesPositionStorage->Unbind(3);
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

	void RenderManager::DrawShadowMap()
	{
		if (mLightData[mShadowMapDebugLightIndex].mpShadowMap)
		{
			// Clear OpenGL for now
			glClear(GL_COLOR_BUFFER_BIT);

			mpShadowDebugShader->Use();
			mpShadowDebugShader->SetInt("shadowMap", 0);
			mLightData[mShadowMapDebugLightIndex].mpShadowMap->TexBind(0, 0);

			DrawFSQ();
		}
	}

	void RenderManager::DebugDisplay()
	{
		if(ImGui::Begin("Renderer"))
		{
			DebugDisplayGBuffer();
			DebugDisplayLighting();
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

	void RenderManager::DebugDisplayLighting()
	{
		if (ImGui::CollapsingHeader("Lighting"))
		{
			//ImGui::InputInt("Shadow Map Debug", &mShadowMapDebugMode);
			ImGui::Checkbox("Shadow Map Debug", &mShadowMapDebugMode);
			ImGui::InputScalar("Shadow Map Light", ImGuiDataType_U32, &mShadowMapDebugLightIndex);
			if (mShadowMapDebugLightIndex >= mLightData.size())
			{
				mShadowMapDebugLightIndex = mLightData.size()-1;
			}
		}
	}
}