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

		// Initialize ambient light shader
		CreateAmbientShader();

		// Initialize local light shader
		CreateLocalLightShader();

		// Init ShadowMap shader
		mpShadowMapShader = new Shader("Resources/Shaders/ShadowMap.vert", "Resources/Shaders/ShadowMap.frag");
		mpShadowDebugShader = new Shader("Resources/Shaders/ShadowDebug.vert", "Resources/Shaders/ShadowDebug.frag");
		mShadowMapMode = 0;

		// Init skydome
		InitializeSkydome();

		// Init Hammersley block
		InitializeHammersley(20);

		// Init blur shader
		mpHorizontalBlurShader = new Shader("Resources/Shaders/HorizontalBlur.comp");
		mpVerticalBlurShader = new Shader("Resources/Shaders/VerticalBlur.comp");

		// Init Debug Shader
		mpDebugShader = new Shader("Resources/Shaders/Debug.vert", "Resources/Shaders/Debug.frag");

		// Init Particle Shader
		mpParticleShader = new Shader("Resources/Shaders/ParticleSystem.vert", "Resources/Shaders/ParticleSystem.frag");
		mpParticleCompute = new Shader("Resources/Shaders/ParticleSystem.compute");
		mpParticlesPositionStorage = new ShaderStorageBuffer();
		mpParticlesPositionStorage->CreateBuffer(MAX_PARTICLES_COUNT * sizeof(glm::vec4));
		GLCall(glEnable(GL_PROGRAM_POINT_SIZE));

		// Init AA Shader
		mpAAShader = new Shader("Resources/Shaders/ShadowDebug.vert", "Resources/Shaders/fxaa.frag");
		mpFinalBuffer = new FrameBuffer(mpWindow->GetWidth(), mpWindow->GetHeight(), 1);
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
			mCameraPosition = camera.mPosition;

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

			// Ambient lighting pass
			AmbientPass();

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

			// Copy depth information from GBuffer
			mpGBuffer->BindRead();
			glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
			glBlitFramebuffer(0, 0, mpWindow->GetWidth(), mpWindow->GetHeight(), 0, 0, mpWindow->GetWidth(), mpWindow->GetHeight(), GL_DEPTH_BUFFER_BIT, GL_NEAREST);
			glBindFramebuffer(GL_FRAMEBUFFER, 0);
			
			// Local lighting pass
			LocalLightingPass();

			// Draw skydome from each camera angle
			DrawSkydome();

			// Post processing effects
			if (mFXAA == 1)
			{
				glBindFramebuffer(GL_READ_FRAMEBUFFER, 0);
				glFramebufferTexture2D(GL_READ_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, 0, 0);
				glReadBuffer(GL_COLOR_ATTACHMENT0);

				glBindFramebuffer(GL_DRAW_FRAMEBUFFER, mpFinalBuffer->GetRendererID());
				glFramebufferTexture2D(GL_DRAW_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D, mpFinalBuffer->mpTextureID[0], 0);
				glDrawBuffer(GL_COLOR_ATTACHMENT1);
				
				glBlitFramebuffer(0, 0, mpWindow->GetWidth(), mpWindow->GetHeight(), 0, 0, mpWindow->GetWidth(), mpWindow->GetHeight(), GL_COLOR_BUFFER_BIT, GL_NEAREST);

				glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, 0, 0);
				glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D, 0, 0);

				mpFinalBuffer->Bind();
				ApplyFXAA();
				mpFinalBuffer->Unbind();
			}
		}

		GLCall(glViewport(0, 0, mpWindow->GetWidth(), mpWindow->GetHeight()));

		// Render ShadowMap as FSQ if a debug mode is set
		if (mShadowMapDebugMode > 0)
		{
			DrawShadowMap();
		}

		// Update ImGui
		//DebugDisplay();
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

	void RenderManager::InitializeSkydome()
	{
		// Create skydome shader
		CreateSkydomeShader();

		// Set skydome size and angles
		mSkydomeData.mRadius = 100.0f;
		mSkydomeData.mAngles = glm::vec3(0.0f, 0.0f, 0.0f);

		// Set exposure and contrast
		mExposure = 1.0f;
		mContrast = 1.0f;
		
		// TODO: Add skydome to game init file
		// Load HDR skydome texture and irradiance map
		mpSkydomeTexture = new Texture("Resources/Skydomes/Newport_Loft_Ref.hdr");
		mpSkydomeIrradianceMap = new Texture("Resources/Skydomes/Newport_Loft_Ref.irr.hdr");
	}

	void RenderManager::InitializeHammersley(unsigned int n)
	{
		const unsigned int maxNumPoints = 100;

		// Create struct to sent to GPU
		struct
		{
			float N;
			float hammersley[2 * maxNumPoints];
		} block;
		block.N = n;

		int kk = 0;
		int pos = 0;

		for (int k = 0; k < n; ++k)
		{
			float p = 0.5f;
			float u = 0.0f;
			for (kk = k; kk; p *= 0.5f)
			{
				if (kk & 1)
				{
					u += p;
				}
				kk = kk >> 1;
			}
			float v = (k + 0.5f) / n;
			block.hammersley[pos++] = u;
			block.hammersley[pos++] = v;
		}

		// Send Hammersley block
		unsigned int id, bindpoint;
		glGenBuffers(1, &id);
		bindpoint = 1;
		glBindBufferBase(GL_UNIFORM_BUFFER, bindpoint, id);
		glBufferData(GL_UNIFORM_BUFFER, sizeof(block), &block, GL_STATIC_DRAW);

		// Attach buffer to shader
		int loc = glGetUniformBlockIndex(mpDeferredShader->mProgram, "HammersleyBlock");
		glUniformBlockBinding(mpDeferredShader->mProgram, loc, bindpoint);

		loc = glGetUniformBlockIndex(mpAmbientShader->mProgram, "HammersleyBlock");
		glUniformBlockBinding(mpAmbientShader->mProgram, loc, bindpoint);
	}

	void RenderManager::CreateAmbientShader()
	{
		mpAmbientShader = new Shader("Resources/Shaders/Ambient.vert", "Resources/Shaders/Ambient.frag");
		mpAmbientShader->Use();
		mpAmbientShader->SetInt("gPosition", 0);
		mpAmbientShader->SetInt("gNormal", 1);
		mpAmbientShader->SetInt("gDiffuse", 2);
		mpAmbientShader->SetInt("gSpecular", 3);
		mpAmbientShader->SetInt("irradianceMap", 4);
		mpAmbientShader->SetInt("hdrMap", 5);
		mpAmbientShader->Unbind();
	}

	void RenderManager::CreateDeferredShader()
	{
		mpDeferredShader = new Shader("Resources/Shaders/Global.vert", "Resources/Shaders/Global.frag");
		mpDeferredShader->Use();
		mpDeferredShader->SetInt("gPosition", 0);
		mpDeferredShader->SetInt("gNormal", 1);
		mpDeferredShader->SetInt("gDiffuse", 2);
		mpDeferredShader->SetInt("gSpecular", 3);
		mpDeferredShader->SetInt("shadowMap", 4);
		mpDeferredShader->SetInt("irradianceMap", 5);
		mpDeferredShader->SetInt("hdrMap", 6);
		mpDeferredShader->Unbind();
	}

	void RenderManager::CreateLocalLightShader()
	{
		mpLocalLightShader = new Shader("Resources/Shaders/LocalLight.vert", "Resources/Shaders/LocalLight.frag");
		mpLocalLightShader->Use();
		mpLocalLightShader->SetInt("gPosition", 0);
		mpLocalLightShader->SetInt("gNormal", 1);
		mpLocalLightShader->SetInt("gDiffuse", 2);
		mpLocalLightShader->SetInt("gSpecular", 3);
		mpLocalLightShader->Unbind();
	}

	void RenderManager::CreateSkydomeShader()
	{
		mpSkydomeShader = new Shader("Resources/Shaders/Skydome.vert", "Resources/Shaders/Skydome.frag");
		mpSkydomeShader->Use();
		mpSkydomeShader->SetInt("skydome", 0);
		mpSkydomeShader->Unbind();
	}

	void RenderManager::CreateShadowMap(LightData& light)
	{
		if (!light.mCastShadow)
		{
			return;
		}

		GLCall(glEnable(GL_DEPTH_TEST));
		GLCall(glDisable(GL_BLEND));
		GLCall(glEnable(GL_CULL_FACE));
		GLCall(glCullFace(GL_FRONT));

		mpShadowMapShader->Use();
		light.mpShadowMap->Bind();

		glm::mat4 LightLookAt, LightProj;
		// Calculate light up vector
		//t0 = glm::normalize(glm::vec3(c->collisionNormal.y, -c->collisionNormal.x, 0.0f));
		LightLookAt = glm::lookAt(light.mPosition, glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		LightProj = glm::perspective(glm::radians(45.0f), (float)mpWindow->GetWidth() / (float)mpWindow->GetHeight(), light.mShadowMapNearPlane, light.mShadowMapFarPlane);

		mpShadowMapShader->SetMat4("Projection", LightProj);
		mpShadowMapShader->SetMat4("View", LightLookAt);

		light.mShadowMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(0.5f)) * glm::scale(glm::mat4(1.0f), glm::vec3(0.5f)) * LightProj * LightLookAt;
		//light.mShadowMatrix = LightProj * LightLookAt;

		//glEnable(GL_CULL_FACE);
		//glCullFace(GL_FRONT);

		DrawShadowCastingObjects(mpShadowMapShader);

		glDisable(GL_CULL_FACE);

		light.mpShadowMap->Unbind();
		GLCall(glDisable(GL_DEPTH_TEST));
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

	void RenderManager::AmbientPass()
	{
		// Bind G-Buffer textures
		mpGBuffer->TexBind(0, 0);
		mpGBuffer->TexBind(1, 1);
		mpGBuffer->TexBind(2, 2);
		mpGBuffer->TexBind(3, 3);

		glDisable(GL_DEPTH_TEST);
		mpAmbientShader->Use();

		mpAmbientShader->SetVec3("viewPosition", mCameraPosition);

		// Bind IBL texutres
		mpSkydomeIrradianceMap->Bind(4);
		mpSkydomeTexture->Bind(5);

		// Send IBL parameters
		mpAmbientShader->SetFloat("exposure", mExposure);
		mpAmbientShader->SetFloat("contrast", mContrast);
		mpAmbientShader->SetFloat("hdrWidth", (float)mpSkydomeTexture->GetWidth());
		mpAmbientShader->SetFloat("hdrHeight", (float)mpSkydomeTexture->GetHeight());

		// Render FSQ
		DrawFSQ();

		mpGBuffer->TexUnbind(0);
		mpGBuffer->TexUnbind(1);
		mpGBuffer->TexUnbind(2);
		mpGBuffer->TexUnbind(3);
		mpSkydomeIrradianceMap->Unbind(4);
		mpSkydomeTexture->Unbind(5);
	}

	void RenderManager::GlobalLightingPass(LightData& light)
	{
		// Clear opengl
		//glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glEnable(GL_BLEND);
		// Bind G-Buffer textures
		mpGBuffer->TexBind(0, 0);
		mpGBuffer->TexBind(1, 1);
		mpGBuffer->TexBind(2, 2);
		mpGBuffer->TexBind(3, 3);
		
		glDisable(GL_DEPTH_TEST);
		mpDeferredShader->Use();

		// Send light and view position
		mpDeferredShader->SetVec3("viewPosition", mCameraPosition);
		mpDeferredShader->SetVec3("lightPosition", light.mPosition);
		mpDeferredShader->SetVec3("lightColor", light.mColor);

		// Send ShadowMap texture and shadow matrix
		light.mpShadowMap->TexBind(0, 4);
		mpDeferredShader->SetInt("shadowMap", 4);
		mpDeferredShader->SetMat4("shadowMatrix", light.mShadowMatrix);
		mpDeferredShader->SetInt("shadowMode", mShadowMapMode);
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
			pShader->SetMat4("NormalTr", /*glm::transpose*/(glm::inverse(data.mpModel)));

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

			pShader->SetInt("hasNormalMap", 0);
			pShader->SetInt("hasHeightMap", 0);
			pShader->SetFloat("heightScale", 0.0f);

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
						pShader->SetInt("hasNormalMap", 1);
					}
					if (materialdata->mpHeight)
					{
						materialdata->mpHeight->Bind(4);
						pShader->SetInt("heightTexture", 4);
						pShader->SetInt("hasHeightMap", 1);
						pShader->SetFloat("heightScale", pMaterial->mHeightScale);
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

	void RenderManager::DrawSkydome()
	{
		glEnable(GL_DEPTH_TEST);
		// Forward render skydome with diffuse only
		mpSkydomeShader->Use();

		// Send MVP matrices
		mpSkydomeShader->SetMat4("Projection", mProjectionMatrix);
		mpSkydomeShader->SetMat4("View", mViewMatrix);

		// Set skydome model matrix
		glm::mat4 skydomeModelMatrix(1.0f);
		skydomeModelMatrix = glm::translate(skydomeModelMatrix, mCameraPosition);
		skydomeModelMatrix = glm::rotate(skydomeModelMatrix, glm::radians(mSkydomeData.mAngles.x), glm::vec3(1.0f, 0.0f, 0.0f));
		skydomeModelMatrix = glm::rotate(skydomeModelMatrix, glm::radians(mSkydomeData.mAngles.y), glm::vec3(0.0f, 1.0f, 0.0f));
		skydomeModelMatrix = glm::rotate(skydomeModelMatrix, glm::radians(mSkydomeData.mAngles.z), glm::vec3(0.0f, 0.0f, 1.0f));
		skydomeModelMatrix = glm::scale(skydomeModelMatrix, glm::vec3(mSkydomeData.mRadius));
		mpSkydomeShader->SetMat4("Model", skydomeModelMatrix);

		mpSkydomeShader->SetFloat("exposure", mExposure);
		mpSkydomeShader->SetFloat("contrast", mContrast);

		mpSkydomeTexture->Bind();

		// Draw a sphere as the skydome
		DrawSphere();

		mpSkydomeTexture->Unbind();
		//glDisable(GL_DEPTH_TEST);
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

			LightData& light = mLightData[mShadowMapDebugLightIndex];
			mpShadowDebugShader->Use();
			mpShadowDebugShader->SetInt("shadowMap", 0);
			mpShadowDebugShader->SetFloat("nearPlane", light.mShadowMapNearPlane);
			mpShadowDebugShader->SetFloat("farPlane", light.mShadowMapFarPlane);
			light.mpShadowMap->TexBind(0, 0);

			DrawFSQ();
		}
	}

	void RenderManager::ApplyFXAA()
	{
		mpAAShader->Use();
		mpAAShader->SetInt("fxaaON", mFXAA);
		mpAAShader->SetFloat("fxaaSpan", mFXAASpan);
		mpAAShader->SetVec2("screenSize", glm::vec2((float)mpWindow->GetWidth(), (float)mpWindow->GetHeight()));
		mpFinalBuffer->TexBind(0, 10);
		mpAAShader->SetInt("finalImage", 10);
		DrawFSQ();
		mpFinalBuffer->TexUnbind(1);
		mpAAShader->Unbind();
	}

	void RenderManager::DebugDisplay()
	{
		DebugDisplayGBuffer();
		DebugDisplayShadow();
		DebugDisplayIBL();
		DebugDisplayAA();
		ImGui::Checkbox("Particle System",&ShowParticles);
	}

	void RenderManager::DebugDisplayGBuffer()
	{
		if (ImGui::CollapsingHeader("GBuffer"))
		{
			ImGui::InputInt("Display Mode", &mGBufferDisplayMode);
		}
	}

	void RenderManager::DebugDisplayShadow()
	{
		if (ImGui::CollapsingHeader("Shadows"))
		{
			ImGui::InputInt("Shadow Map Mode", &mShadowMapMode);
			ImGui::Checkbox("Shadow Map Debug", &mShadowMapDebugMode);
			ImGui::InputScalar("Shadow Map Light", ImGuiDataType_U32, &mShadowMapDebugLightIndex);
			if (mShadowMapDebugLightIndex >= mLightData.size())
			{
				mShadowMapDebugLightIndex = mLightData.size()-1;
			}
		}
	}

	void RenderManager::DebugDisplayIBL()
	{
		if (ImGui::CollapsingHeader("IBL"))
		{
			ImGui::InputFloat("Exposure", &mExposure);
			ImGui::InputFloat("Contrast", &mContrast);
			ImGui::Image((void*)mpSkydomeTexture->GetRendererID(), ImVec2(ImGui::GetContentRegionAvailWidth(), ImGui::GetContentRegionAvailWidth()), ImVec2(1, 1), ImVec2(0, 0));
			ImGui::Image((void*)mpSkydomeIrradianceMap->GetRendererID(), ImVec2(ImGui::GetContentRegionAvailWidth(), ImGui::GetContentRegionAvailWidth()), ImVec2(1, 1), ImVec2(0, 0));
		}
	}
	void RenderManager::DebugDisplayAA()
	{
		if (ImGui::CollapsingHeader("AA"))
		{
			ImGui::InputInt("FXAA On", &mFXAA);
			ImGui::InputFloat("FXAA Span", &mFXAASpan);
		}
	}
}