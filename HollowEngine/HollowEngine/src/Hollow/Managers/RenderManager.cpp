#include <hollowpch.h>
#include "Hollow/Common.h"
#include "RenderManager.h"

#include "ImGuiManager.h"

#include "Hollow/Components/Material.h"
#include "Hollow/Components/ParticleEmitter.h"

#include "Hollow/Graphics/GameWindow.h"
#include "Hollow/Graphics/Shader.h"
#include "Hollow/Graphics/VertexBuffer.h"
#include "Hollow/Graphics/VertexArray.h"
#include "Hollow/Graphics/ElementArrayBuffer.h"
#include "Hollow/Graphics/Mesh.h"
#include "Hollow/Graphics/Texture.h"
#include "Hollow/Graphics/FrameBuffer.h"
#include "Hollow/Graphics/ShaderStorageBuffer.h"
#include "Hollow/Graphics/UniformBuffer.h"
#include "Hollow/Graphics/TextRenderer.h"

#include "Hollow/Managers/FrameRateController.h"
#include "Hollow/Managers/PhysicsManager.h"
#include "Hollow/Managers/DebugDrawManager.h"

#include "ResourceManager.h"

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

		// Initialize ambient light shader
		CreateAmbientShader();

		// Initialize local light shader
		CreateLocalLightShader(data);

		// Init ShadowMap shader
		mpShadowMapShader = new Shader(data["ShadowMapShader"].GetArray()[0].GetString(), data["ShadowMapShader"].GetArray()[1].GetString());
		mpShadowDebugShader = new Shader(data["DebugShadowMapShader"].GetArray()[0].GetString(), data["DebugShadowMapShader"].GetArray()[1].GetString());
		mShadowMapMode = 0;

		// Init skydome
		InitializeSkydome(data);

		// Init Hammersley block
		InitializeHammersley(20);

		// Init blur shader
		mpHorizontalBlurShader = new Shader(data["BlurComputeShaders"].GetArray()[0].GetString());
		mpVerticalBlurShader = new Shader(data["BlurComputeShaders"].GetArray()[1].GetString());
		mpWeights = new UniformBuffer(101 * sizeof(float));
		horizontalBlurred = new Texture(4, 1024, 1024);

		// Init Debug Shader
		mpDebugShader = new Shader(data["DebugShader"].GetArray()[0].GetString(), data["DebugShader"].GetArray()[1].GetString());
		mShowDebugDrawing = false;

		// Init Particle Shader
		// Point Particles
		mpParticleShader = new Shader(data["ParticleShader"].GetArray()[0].GetString(), data["ParticleShader"].GetArray()[1].GetString());
		mpParticlesPositionStorage = new ShaderStorageBuffer();
		mpParticlesPositionStorage->CreateBuffer(MAX_PARTICLES_COUNT * sizeof(glm::vec4));
		ShowParticles = true;
		GLCall(glEnable(GL_PROGRAM_POINT_SIZE));

		// Model Particles
		if (data.HasMember("ModelParticleShader"))
		{
			mpModelParticleShader = new Shader(data["ModelParticleShader"].GetArray()[0].GetString(), data["ModelParticleShader"].GetArray()[1].GetString());
			mpParticlesModelStorage = new ShaderStorageBuffer();
			mpParticlesModelStorage->CreateBuffer(MAX_PARTICLES_COUNT * 0.5f * (sizeof(glm::mat4) + sizeof(glm::vec4)));
		}
		else
		{
			mpModelParticleShader = mpParticleShader;
		}

		// Init AA Shader
		mpAAShader = new Shader("Resources/Shaders/ShadowDebug.vert", "Resources/Shaders/fxaa.frag");
		mpFinalBuffer = new FrameBuffer(mpWindow->GetWidth(), mpWindow->GetHeight(), 1, true);

		// Init Bloom Shader and FrameBuffer
		mpBloomShader = new Shader(data["BloomShader"].GetArray()[0].GetString(), data["BloomShader"].GetArray()[1].GetString());
		mpBloomFrame = new FrameBuffer(mpWindow->GetWidth(), mpWindow->GetHeight(), 2, true);

		// Init UI Shader
		mpUIShader = new Shader(data["UIShader"].GetArray()[0].GetString(), data["UIShader"].GetArray()[1].GetString());
		mpUITextShader = new Shader(data["UITextShader"].GetArray()[0].GetString(), data["UITextShader"].GetArray()[1].GetString());

		// Init Text Renderer
		mpTextRenderer = new TextRenderer();

		// Add to ImGui debug display
		ImGuiManager::Instance().AddDisplayFunction("Renderer", std::bind(&RenderManager::DebugDisplay, &RenderManager::Instance()));
	}

	void RenderManager::CleanUp()
	{
		delete mpTextRenderer;
		delete mpDeferredShader;
		delete mpUIShader;
		// CleanUp GBuffer things
		delete mpGBufferShader;
		delete mpGBuffer;
		delete mpWeights;
		delete horizontalBlurred;
	}
	//TODO: Remove before finishing the game
	void DebugContacts()
	{
		ImGui::Begin("Contacts");
		{
			for (auto points : *PhysicsManager::Instance().mSAT.mPrevContacts)
			{
				for (int i = 0; i < points->contactPoints.size(); i++)
				{
					DebugDrawManager::Instance().DebugCube(points->contactPoints[i].point, glm::vec3(0.05f), glm::vec3(1.0f));
					ImGui::Text("Point location- %f , %f , %f ", points->contactPoints[i].point.x, points->contactPoints[i].point.y, points->contactPoints[i].point.z);
					ImGui::Text("Penetration depth - %f", points->contactPoints[i].penetrationDepth);
				}
			}
		}
		ImGui::End();
	}
	void RenderManager::Update()
	{
		GLCall(glClearColor(0.0f, 0.0f, 0.0f, 1.0f));
		GLCall(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));

		for (unsigned int i = 0; i < mLightData.size(); ++i)
		{
			if (mLightData[i].mCastShadow)
			{

			// ShadowMap Pass
			CreateShadowMap(mLightData[i]);
			// Blur ShadowMap
			FrameBuffer& shadowmap = *mLightData[i].mpShadowMap;
			BlurTexture(shadowmap.mpTextureID[0],
				shadowmap.mWidth,
				shadowmap.mHeight, 4,
				mLightData[i].mBlurDistance,
				shadowmap.mpTextureID[0]);
			}
		}

		// Draw Main Camera
		mProjectionMatrix = mMainCamera.mProjectionMatrix;
		mViewMatrix = mMainCamera.mViewMatrix;
		mCameraPosition = mMainCamera.mEyePosition;

		GLCall(glViewport(0, 0, mMainCamera.mViewPortSize.x, mMainCamera.mViewPortSize.y));

		// Deferred G-Buffer Pass
		GBufferPass();// mMainCamera);

		// Bloom Capture Start
		if (mBloomEnabled)
		{
			mpBloomFrame->Bind();
		}
		else if (mFXAA == 1)
		{
			mpFinalBuffer->Bind();
		}

		// Ambient lighting pass
		AmbientPass();

		for (unsigned int i = 0; i < mLightData.size(); ++i)
		{
			if (mLightData[i].mCastShadow)
			{

			// Apply global lighting
			GlobalLightingPass(mLightData[i]);// , mMainCamera.mEyePosition);
			}
		}

		glBindFramebuffer(GL_READ_FRAMEBUFFER, mpGBuffer->GetFrameBufferID());
		if (mBloomEnabled)
		{
			glBindFramebuffer(GL_DRAW_FRAMEBUFFER, mpBloomFrame->GetFrameBufferID());
		}
		else if (mFXAA == 1)
		{
			glBindFramebuffer(GL_DRAW_FRAMEBUFFER, mpFinalBuffer->GetFrameBufferID());
		}
		else
		{
			glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);// write to default framebuffer
		}
		glBlitFramebuffer(0, 0, mpGBuffer->mWidth, mpGBuffer->mHeight,
			0, 0, mpGBuffer->mWidth, mpGBuffer->mHeight, GL_DEPTH_BUFFER_BIT, GL_NEAREST);

		glEnable(GL_DEPTH_TEST);

		// Draw skydome from each camera angle
		DrawSkydome();

		if (ShowParticles)
		{
			DrawParticles(mMainCamera);
		}

		//Bloom capture End
		if (mBloomEnabled)
		{
			mpBloomFrame->Unbind();
			if (mFXAA == 1)
			{
				mpFinalBuffer->Bind();
			}
			DrawSceneWithBloom();
		}

		// Draw Secondary Cameras
		//for (unsigned i = 0; i < mSecondaryCameras.size(); ++i)
		//{
		//	mProjectionMatrix = mSecondaryCameras[i].mProjectionMatrix;
		//	mViewMatrix = mSecondaryCameras[i].mViewMatrix;

		//	GLCall(glViewport(mSecondaryCameras[i].mViewPortPosition.x, mSecondaryCameras[i].mViewPortPosition.y,
		//		mSecondaryCameras[i].mViewPortSize.x, mSecondaryCameras[i].mViewPortSize.y));

		//	// Deferred G-Buffer Pass
		//	GBufferPass();// mSecondaryCameras[i]);

		//	// Ambient lighting pass
		//	AmbientPass();

		//	// Bloom Capture Start
		//	if (mBloomEnabled)
		//	{
		//		mpBloomFrame->Bind();
		//	}

		//	for (unsigned int i = 0; i < mLightData.size(); ++i)
		//	{
		//		// Apply global lighting
		//		GlobalLightingPass(mLightData[i]);// , mSecondaryCameras[i].mEyePosition);
		//	}

		//	if (ShowParticles)
		//	{
		//		DrawParticles(mSecondaryCameras[i]);
		//	}

		//	//Bloom capture End
		//	if (mBloomEnabled)
		//	{
		//		mpBloomFrame->Unbind();
		//		DrawSceneWithBloom();
		//	}
		//}
		// Copy depth information from GBuffer
		mpGBuffer->BindRead();
		if (mFXAA == 1)
		{
			glBindFramebuffer(GL_DRAW_FRAMEBUFFER, mpFinalBuffer->GetFrameBufferID());
		}
		else
		{
			glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
		}
		glBlitFramebuffer(0, 0, mpWindow->GetWidth(), mpWindow->GetHeight(), 0, 0, mpWindow->GetWidth(), mpWindow->GetHeight(), GL_DEPTH_BUFFER_BIT, GL_NEAREST);
		//glBindFramebuffer(GL_FRAMEBUFFER, 0);

		// Post processing effects
		if (mFXAA == 1)
		{
			mpFinalBuffer->Unbind();
			
			ApplyFXAA();
		}

		// Local lighting pass
		LocalLightingPass();

		//Draw debug drawings
		if (mShowDebugDrawing || mShowPartialDebug)
		{
			DrawDebugDrawings();
		}


		GLCall(glViewport(0, 0, mpWindow->GetWidth(), mpWindow->GetHeight()));

		// Render ShadowMap as FSQ if a debug mode is set
		if (mShadowMapDebugMode)
		{
			DrawShadowMap();
		}

		// UI camera stuff
		DrawUI();

		// Update ImGui
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		ImGuiManager::Instance().Update();

		SDL_GL_SwapWindow(mpWindow->GetWindow());

		mParticleData.clear();
		mLightData.clear();
		mRenderData.clear();
		mSecondaryCameras.clear();
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


	void RenderManager::InitializeSkydome(rapidjson::Value::Object& data)
	{
		// Create skydome shader
		CreateSkydomeShader();

		// Set skydome size and angles
		mSkydomeData.mRadius = 1000.0f;
		mSkydomeData.mAngles = glm::vec3(0.0f, 0.0f, 0.0f);

		// Set exposure and contrast
		if(data.HasMember("Exposure"))
		{
			mExposure = data["Exposure"].GetFloat();
		}
		else
		{
			mExposure = 1.0f;
		}
		if(data.HasMember("Contrast"))
		{
			mContrast = data["Contrast"].GetFloat();
		}
		else
		{
			mContrast = 1.0f;
		}

		// TODO: Add skydome to game init file
		// Load HDR skydome texture and irradiance map
		mpSkydomeTexture = new Texture(data["Skydome"].GetArray()[0].GetString());
		mpSkydomeIrradianceMap = new Texture(data["Skydome"].GetArray()[1].GetString());
	}

	void RenderManager::InitializeHammersley(int n)
	{
		const unsigned int maxNumPoints = 100;

		// Create struct to sent to GPU
		struct
		{
			float N;
			float hammersley[2 * maxNumPoints];
		} block;
		block.N = static_cast<float>(n);

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


	void RenderManager::CreateDeferredShader(rapidjson::Value::Object& data)
	{
		mpDeferredShader = new Shader(data["DeferredShader"].GetArray()[0].GetString(), data["DeferredShader"].GetArray()[1].GetString());
		mpDeferredShader->Use();
		mpDeferredShader->SetInt("gPosition", 0);
		mpDeferredShader->SetInt("gNormal", 1);
		mpDeferredShader->SetInt("gDiffuse", 2);
		mpDeferredShader->SetInt("gSpecular", 3);
		mpDeferredShader->SetInt("shadowMap", 4);
		mpDeferredShader->Unbind();
	}

	void RenderManager::CreateLocalLightShader(rapidjson::Value::Object& data)
	{
		mpLocalLightShader = new Shader(data["LocalLightShader"].GetArray()[0].GetString(), data["LocalLightShader"].GetArray()[1].GetString());
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
		LightLookAt = glm::lookAt(light.mPosition, light.mLookAtPoint, glm::vec3(0.0f, 1.0f, 0.0f));
		LightProj = glm::perspective(glm::radians(light.mFOV), (float)mpWindow->GetWidth() / (float)mpWindow->GetHeight(), light.mShadowMapNearPlane, light.mShadowMapFarPlane);

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

	void RenderManager::BlurTexture(unsigned int inputTextureID, unsigned int width, unsigned int height, unsigned int channels, unsigned int blurWidth, unsigned int& outputTextureID)
	{
		// Create blur kernel
		std::vector<float> weights = CreateBlurKernel(blurWidth);

		// Apply horizontal blur
		// Make horizontal blur shader current
		mpHorizontalBlurShader->Use();

		// Send block of weights to shader as uniform block
		mpWeights->Bind(2);
		mpWeights->SubData(static_cast<unsigned>(sizeof(float) * weights.size()), &weights[0]);

		// Send texture to shader
		mpHorizontalBlurShader->SetInputUniformImage("src", inputTextureID, 0, channels);
		mpHorizontalBlurShader->SetOutputUniformImage("dst", horizontalBlurred->GetTextureID(), 1, channels);
		// Send blur distance to shader
		mpHorizontalBlurShader->SetUInt("blurDistance", blurWidth);
		mpHorizontalBlurShader->SetUniformBlock("blurKernel", 2);

		mpHorizontalBlurShader->DispatchCompute(width / 128, height, 1);
		mpHorizontalBlurShader->Unbind();

		// Apply vertical blur
		// Make vertical blur shader current
		mpVerticalBlurShader->Use();

		// Send texture to shader
		mpVerticalBlurShader->SetInputUniformImage("src", horizontalBlurred->GetTextureID(), 0, channels);
		mpVerticalBlurShader->SetOutputUniformImage("dst", outputTextureID, 1, channels);
		// Send blur distance to shader
		mpVerticalBlurShader->SetUInt("blurDistance", blurWidth);
		// Send block of weights to shader as uniform block
		mpVerticalBlurShader->SetUniformBlock("blurKernel", 2);

		mpVerticalBlurShader->DispatchCompute(width, height / 128, 1);
		mpVerticalBlurShader->Unbind();
		mpWeights->Unbind();
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

	void RenderManager::GBufferPass()//CameraData& cameraData)
	{
		GLCall(glEnable(GL_DEPTH_TEST));
		mpGBuffer->Bind();
		//glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		mpGBufferShader->Use();

		// Send view and projection matrix
		//mpGBufferShader->SetMat4("View", cameraData.mViewMatrix);
		//mpGBufferShader->SetMat4("Projection", cameraData.mProjectionMatrix);
		mpGBufferShader->SetMat4("View", mViewMatrix);
		mpGBufferShader->SetMat4("Projection", mProjectionMatrix);

		// Draw all game objects
		DrawAllRenderData(mpGBufferShader);

		//Clear Transparent Objects list
		mTransparentObjects.clear();

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
		mpAmbientShader->SetInt("bloomEnabled", mBloomEnabled);

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
		glBlendFunc(GL_ONE, GL_ONE);
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
		mpDeferredShader->SetInt("bloomEnabled", mBloomEnabled);

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
			mpLocalLightShader->SetMat4("Projection", mMainCamera.mProjectionMatrix);
			mpLocalLightShader->SetMat4("View", mMainCamera.mViewMatrix);
			mpLocalLightShader->SetVec3("viewPosition", mMainCamera.mEyePosition);

			// Draw light volume
			for (auto& light : mLightData)
			{
				mpLocalLightShader->SetFloat("lightRadius", light.mRadius);
				mpLocalLightShader->SetVec3("lightPosition", light.mPosition);
				mpLocalLightShader->SetVec3("lightColor", light.mColor);

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
			if (data.mpMaterial->mTransparent)
			{
				mTransparentObjects.push_back(data);
			}
			else
			{
				DrawRenderData(data, pShader);
			}
		}

		glEnable(GL_BLEND);
		glBlendFunc(GL_ONE,GL_ONE);
		glEnable(GL_CULL_FACE);
		glCullFace(GL_BACK);
		glDepthMask(GL_FALSE);

		for (RenderData& data : mTransparentObjects)
		{
			DrawRenderData(data, pShader);
		}

		glDisable(GL_BLEND); 
		glDisable(GL_CULL_FACE);
		glDepthMask(GL_TRUE);
	}

	void RenderManager::DrawRenderData(RenderData& data, Shader* pShader)
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
		pShader->SetFloat("alphaValue", pMaterial->mAlphaValue);
		pShader->SetVec3("specularColor", pMaterial->mSpecularColor);
		pShader->SetFloat("shininess", pMaterial->mShininess);
		pShader->SetInt("hasDiffuseTexture", 0);

		// Send diffuse texture information
		if (pMaterial->mpTexture)
		{
			pMaterial->mpTexture->Bind(1);
			pShader->SetInt("diffuseTexture", 1);
			pShader->SetInt("hasDiffuseTexture", 1);
		}

		pShader->SetInt("hasNormalMap", 0);
		pShader->SetInt("hasHeightMap", 0);
		pShader->SetFloat("heightScale", 0.0f);
		pShader->SetInt("hasSpecularTexture", 0);

		// Draw object
		for (Mesh* mesh : data.mpMeshes)
		{
			if (mesh->mMaterialIndex != -1 && pMaterial->mMaterials.size() > 0)
			{
				MaterialData* materialdata = pMaterial->mMaterials[mesh->mMaterialIndex];
				if (materialdata->mpDiffuse)
				{
					materialdata->mpDiffuse->Bind(1);
					pShader->SetInt("diffuseTexture", 1);
					pShader->SetInt("hasDiffuseTexture", 1);
				}
				if (materialdata->mpSpecular)
				{
					materialdata->mpSpecular->Bind(2);
					pShader->SetInt("specularTexture", 2);
					pShader->SetInt("hasSpecularTexture", 1);
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

			if (mesh->mMaterialIndex != -1 && pMaterial->mMaterials.size() > 0)
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

	void RenderManager::DrawParticles(CameraData& cameraData)
	{		

		// Draw Particles 
		mpParticleShader->Use();
		mpParticleShader->SetMat4("View", cameraData.mViewMatrix);
		mpParticleShader->SetMat4("Projection", cameraData.mProjectionMatrix);

		mpModelParticleShader->Use();
		mpModelParticleShader->SetMat4("View", cameraData.mViewMatrix);
		mpModelParticleShader->SetMat4("Projection", cameraData.mProjectionMatrix);

		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		for (unsigned int i = 0; i < mParticleData.size(); ++i)
		{
			ParticleData& particle = mParticleData[i];
			
			particle.emitter->mpParticleStorage->Bind(2);
			if (particle.emitter->mType == POINT)
			{
				mpParticlesPositionStorage->Bind(3);
			}
			else if (particle.emitter->mType == MODEL)
			{
				mpParticlesModelStorage->Bind(3);
			}
			
			particle.emitter->mpComputeShader->Use();
			particle.emitter->mpComputeShader->SetInt("Pause", PauseParticles);
			particle.emitter->mpComputeShader->SetInt("Active", particle.emitter->mActive);
			particle.emitter->mpComputeShader->SetVec3("Center", particle.emitter->mCenterOffset);
			particle.emitter->mpComputeShader->SetFloat("DeltaTime", FrameRateController::Instance().GetFrameTime());
			particle.emitter->mpComputeShader->SetVec2("SpeedRange", particle.emitter->mSpeedRange);
			particle.emitter->mpComputeShader->SetVec2("LifeRange", particle.emitter->mLifeRange);
			particle.emitter->mpComputeShader->SetVec2("ScaleRange", particle.emitter->mSizeRange);
			particle.emitter->mpComputeShader->SetVec3("Direction", particle.emitter->mDirection);
			particle.emitter->mpComputeShader->DispatchCompute(particle.emitter->mMaxCount / 128, 1, 1);
			ShaderStorageBuffer::PutMemoryBarrier();
			particle.emitter->mpComputeShader->Unbind();
			particle.emitter->mpParticleStorage->Unbind(2);

			if (particle.emitter->mType == POINT)
			{
				mpParticleShader->Use();
				mpParticleShader->SetMat4("Model", particle.emitter->mModelMatrix);
				mpParticleShader->SetVec2("ScreenSize", glm::vec2(mpWindow->GetWidth(), mpWindow->GetHeight()));
				mpParticleShader->SetFloat("SpriteSize", particle.emitter->mPixelSize);

				particle.emitter->mTexture->Bind(4);
				mpParticleShader->SetInt("Texx", 4);
				particle.emitter->mpParticleVAO->Bind();

				GLCall(glDrawArrays(GL_POINTS, 0, particle.emitter->mCount));
				mpParticleShader->Unbind();
				particle.emitter->mTexture->Unbind(4);
				mpParticlesPositionStorage->Unbind(3);
				particle.emitter->mpParticleVAO->Unbind();
			}
			else if (particle.emitter->mType == MODEL)
			{				
				mpModelParticleShader->Use();
				mpModelParticleShader->SetMat4("Model", particle.emitter->mModelMatrix);
				particle.emitter->mpParticleVAO->Bind();
				for (Mesh* mesh : particle.emitter->mParticleModel)
				{
					if (mesh->mMaterialIndex != -1 && particle.emitter->mParticleMaterials.size() > 0)
					{
						MaterialData* materialdata = particle.emitter->mParticleMaterials[mesh->mMaterialIndex];
						if (materialdata && materialdata->mpDiffuse)
						{
							materialdata->mpDiffuse->Bind(4);
							mpModelParticleShader->SetInt("DiffuseTexture", 4);
							mpModelParticleShader->SetInt("HasTexture", 1);
						}						
					}
					else
					{
						mpModelParticleShader->SetInt("HasTexture", 0);
						mpModelParticleShader->SetVec3("DiffuseColor", particle.emitter->mParticleColor);
					}
					mesh->mpVAO->Bind();
					mesh->mpVBO->Bind();
					mesh->mpEBO->Bind();
					GLCall(glDrawElementsInstanced(GL_TRIANGLES, mesh->mpEBO->GetCount(), GL_UNSIGNED_INT, 0, particle.emitter->mCount));
					mesh->mpEBO->Unbind();
					mesh->mpVBO->Unbind();
					mesh->mpVAO->Unbind();
					if (mesh->mMaterialIndex != -1 && particle.emitter->mParticleMaterials.size() > 0)
					{
						MaterialData* materialdata = particle.emitter->mParticleMaterials[mesh->mMaterialIndex];
						if (materialdata->mpDiffuse)
						{
							materialdata->mpDiffuse->Unbind(4);
						}
					}
				}
				mpModelParticleShader->Unbind();
				mpParticlesModelStorage->Unbind(3);
				particle.emitter->mpParticleVAO->Unbind();
			}
		}
		glDisable(GL_BLEND);
		glDisable(GL_DEPTH_TEST);
	}

	void RenderManager::DrawSkydome()
	{
		glEnable(GL_DEPTH_TEST);
		glDisable(GL_BLEND);
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
		mpSkydomeShader->SetInt("bloomEnabled", mBloomEnabled);

		mpSkydomeTexture->Bind();

		// Draw a sphere as the skydome
		DrawSphere();

		mpSkydomeTexture->Unbind();
		//glDisable(GL_DEPTH_TEST);
	}

	void RenderManager::DrawDebugDrawings()
	{
		DebugContacts();
		mpDebugShader->Use();
		mpDebugShader->SetMat4("View", mMainCamera.mViewMatrix);
		mpDebugShader->SetMat4("Projection", mMainCamera.mProjectionMatrix);

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

		mpDebugShader->SetMat4("Model", glm::mat4(1.0f));
		mpDebugShader->SetVec3("Color", COLOR_BLUE);

		for (unsigned int i = 0; i < mDebugPathData.size(); ++i)
		{
			DebugPathData& path = mDebugPathData[i];
			path.mCurveVAO->Bind();
			//glLineWidth(2.0f);
			glDrawArrays(GL_LINES, 1, path.mCurvePointsCount - 1);
			path.mCurveVAO->Unbind();
		}

		mDebugPathData.clear();

		mpDebugShader->Unbind();
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
		mpFinalBuffer->TexBind(0, 0);
		mpAAShader->SetInt("finalImage", 0);
		DrawFSQ();
		mpFinalBuffer->TexUnbind(0);
		mpAAShader->Unbind();
	}

	void RenderManager::DrawSceneWithBloom()
	{
		glEnable(GL_BLEND);
		glBlendFunc(GL_ONE, GL_ONE);
		BlurTexture(mpBloomFrame->mpTextureID[1],
			mpBloomFrame->mWidth,
			mpBloomFrame->mHeight,
			4, 5, mpBloomFrame->mpTextureID[1]);

		GLCall(glClear(GL_COLOR_BUFFER_BIT));
		
		mpBloomShader->Use();
		mpBloomFrame->TexBind(0, 1);
		mpBloomShader->SetInt("scene", 1);
		mpBloomFrame->TexBind(1, 2);
		mpBloomShader->SetInt("blur", 2);
		mpBloomShader->SetFloat("exposure", mExposure);
		mpBloomShader->SetFloat("contrast", mContrast);
		DrawFSQ();
		mpBloomShader->Unbind();
		glDisable(GL_BLEND);
	}

	void RenderManager::DrawUI()
	{
		glm::mat4& mProjectionMatrix = mUICamera.mProjectionMatrix;
		glm::mat4& mViewMatrix = mUICamera.mViewMatrix;

		GLCall(glViewport(0, 0, mUICamera.mViewPortSize.x, mUICamera.mViewPortSize.y));

		mpUIShader->Use();
		mpUIShader->SetMat4("Projection", mProjectionMatrix);

		GLCall(glEnable(GL_BLEND));
		GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));

		for (unsigned int i = 0; i < mUIRenderData.size(); ++i)
		{
			UIRenderData& uidata = mUIRenderData[i];

			if (uidata.mpTexture)
			{
				uidata.mpTexture->Bind(1);
				mpUIShader->SetInt("UITexture", 1);
				mpUIShader->SetInt("hasTexture", 1);
			}
			else
			{
				mpUIShader->SetVec3("UIColor", uidata.mColor);
				mpUIShader->SetInt("hasTexture", 0);
			}
			mpUIShader->SetMat4("Model", uidata.mModelTransform);

			Mesh* shape = uidata.mpShape;

			shape->mpVAO->Bind();
			shape->mpEBO->Bind();
			shape->mpVBO->Bind();
			GLCall(glDrawElements(GL_TRIANGLES, shape->mpEBO->GetCount(), GL_UNSIGNED_INT, 0));
			shape->mpEBO->Unbind();
			shape->mpVBO->Unbind();
			shape->mpVAO->Unbind();

			if (uidata.mpTexture)
			{
				uidata.mpTexture->Unbind(1);
			}
		}

		mpUIShader->Unbind();
		mUIRenderData.clear();

		// Render UI Text
		mpUITextShader->Use();
		mpUITextShader->SetMat4("Projection", mProjectionMatrix);
		
		for (int i = 0; i < mUITextData.size(); ++i)
		{	
			mpTextRenderer->RenderText(mUITextData[i],mpUITextShader);
		}
		
		mpUITextShader->Unbind();
		mUITextData.clear();

		GLCall(glDisable(GL_BLEND));
	}

	void RenderManager::DebugDisplay()
	{
		DebugDisplayGBuffer();
		DebugDisplayShadow();
		DebugDisplayIBL();
		DebugDisplayPostProcessing();
		ImGui::Checkbox("Particle System", &ShowParticles);
		ImGui::Checkbox("Pause Particle System", &PauseParticles);
		ImGui::Checkbox("Show Debug", &mShowDebugDrawing);
		ImGui::Checkbox("Show Partial Debug", &mShowPartialDebug);
		
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
				mShadowMapDebugLightIndex = static_cast<unsigned>(mLightData.size() - 1);
			}
		}
	}

	void RenderManager::DebugDisplayIBL()
	{
		if (ImGui::CollapsingHeader("IBL"))
		{
			ImGui::InputFloat("Exposure", &mExposure);
			ImGui::InputFloat("Contrast", &mContrast);
			ImGui::Image((void*)(intptr_t)mpSkydomeTexture->GetTextureID(), ImVec2(ImGui::GetContentRegionAvailWidth(), ImGui::GetContentRegionAvailWidth()), ImVec2(1, 1), ImVec2(0, 0));
			ImGui::Image((void*)(intptr_t)mpSkydomeIrradianceMap->GetTextureID(), ImVec2(ImGui::GetContentRegionAvailWidth(), ImGui::GetContentRegionAvailWidth()), ImVec2(1, 1), ImVec2(0, 0));
		}
	}
	void RenderManager::DebugDisplayPostProcessing()
	{
		if (ImGui::CollapsingHeader("PostProcessing"))
		{
			ImGui::Checkbox("Bloom", &mBloomEnabled);
			ImGui::InputInt("FXAA On", &mFXAA);
			ImGui::InputFloat("FXAA Span", &mFXAASpan);
		}
	}
}