#pragma once

#include "Utils/Singleton.h"
#include "Hollow/Graphics/Data/RenderData.h"
#include "Hollow/Graphics/Data/LightData.h"
#include "Hollow/Graphics/Data/MaterialData.h"
#include "Hollow/Graphics/Data/DebugRenderData.h"
#include "Hollow/Graphics/Data/CameraData.h"
#include "Hollow/Graphics/Data/ParticleData.h"
#include "Hollow/Graphics/Data/SkydomeData.h"
#include "Hollow/Graphics/Data/UIRenderData.h"
#include <GL/glew.h>

namespace Hollow {

	class GameWindow;
	//class Camera;
	class Shader;
	class FrameBuffer;
	class UniformBuffer;

	class HOLLOW_API RenderManager
	{
		SINGLETON(RenderManager)
	public:
		void Init(rapidjson::Value::Object& data, GameWindow* pWindow = nullptr);
		void CleanUp();
		void Update();
		inline glm::vec2 GetWindowSize();
		void DebugDisplay();

	private:
		// Initialization Functions
		void InitializeSkydome();
		void InitializeHammersley(unsigned int n);

		void CreateAmbientShader();
		void CreateLocalLightShader(rapidjson::Value::Object& data);
		void CreateDeferredShader(rapidjson::Value::Object& data);
		void CreateSkydomeShader();

		void InitializeGBuffer(rapidjson::Value::Object& data);


		void CreateShadowMap(LightData& light);
		void BlurTexture(unsigned int inputTextureID, unsigned int width, unsigned int height, unsigned int channels, unsigned int blurWidth, unsigned int& outputTextureID);
		std::vector<float> CreateBlurKernel(unsigned int distance);

		void GBufferPass();
		void AmbientPass();
		void GlobalLightingPass(LightData& light);
		void LocalLightingPass();

		void DrawAllRenderData(Shader* pShader);
		void DrawShadowCastingObjects(Shader* pShader);
		void DrawFSQ();
		void DrawSphere();

		void DrawParticles(CameraData& cameraData);

		void DrawSkydome();

		void DrawDebugDrawings();

		void DrawShadowMap();

		void ApplyFXAA();
		void DrawSceneWithBloom();

		void DrawUI();

		// ImGui Debug functions
		void DebugDisplayGBuffer();
		void DebugDisplayShadow();
		void DebugDisplayIBL();
		void DebugDisplayPostProcessing();

	public:
		std::vector<RenderData> mRenderData;
		std::vector<DebugRenderData> mDebugRenderData;
		std::vector<LightData> mLightData;
		std::vector<ParticleData> mParticleData;
		std::vector<DebugPathData> mDebugPathData;
		std::vector<UIRenderData> mUIRenderData;
		
		CameraData mMainCamera;
		CameraData mUICamera;
		std::vector<CameraData> mSecondaryCameras;

	private:
		// Transformation matricies
		glm::mat4 mProjectionMatrix;
		glm::mat4 mViewMatrix;

		GameWindow* mpWindow;
		//Camera* mpCamera;
		glm::vec3 mCameraPosition;

		// Lighting
		Shader* mpDeferredShader;
		Shader* mpLocalLightShader;

		// Skydome
		Shader* mpSkydomeShader;
		Texture* mpSkydomeTexture;
		Texture* mpSkydomeIrradianceMap;
		SkydomeData mSkydomeData;

		// Ambient lighting pass
		Shader* mpAmbientShader;

		// Image based lighting
		float mExposure;
		float mContrast;

		// Debug drawing Shader
		Shader* mpDebugShader;

		// G-Buffer
		Shader* mpGBufferShader;
		FrameBuffer* mpGBuffer;
		int mGBufferDisplayMode;

		// Shadows
		Shader* mpShadowMapShader;
		Shader* mpShadowDebugShader;
		bool mShadowMapDebugMode;
		int mShadowMapMode;
		unsigned int mShadowMapDebugLightIndex;

		// Blur
		UniformBuffer* mpWeights;
		Shader* mpHorizontalBlurShader;
		Shader* mpVerticalBlurShader;

		// ParticleSystem
		Shader* mpParticleShader;
		ShaderStorageBuffer* mpParticlesPositionStorage;
		bool ShowParticles;

		// Post-Processing
		Shader* mpAAShader;
		FrameBuffer* mpFinalBuffer;
		int mFXAA;
		float mFXAASpan;

		// Bloom
		Shader* mpBloomShader;
		FrameBuffer* mpBloomFrame;
		bool mBloomEnabled;

		// UI Shader
		Shader* mpUIShader;
	};
}