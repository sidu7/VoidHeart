#pragma once

#include "Utils/Singleton.h"
#include "Hollow/Graphics/Data/RenderData.h"
#include "Hollow/Graphics/Data/LightData.h"
#include "Hollow/Graphics/Data/MaterialData.h"
#include "Hollow/Graphics/Data/DebugRenderData.h"
#include "Hollow/Graphics/Data/CameraData.h"
#include "Hollow/Graphics/Data/ParticleData.h"
#include "Hollow/Graphics/Data/SkydomeData.h"
#include <GL/glew.h>

namespace Hollow {

	class GameWindow;
	//class Camera;
	class Shader;
	class FrameBuffer;

	class HOLLOW_API RenderManager
	{
		SINGLETON(RenderManager)
	public:
		void Init(GameWindow* pWindow = nullptr);
		void CleanUp();
		void Update();
		inline glm::vec2 GetWindowSize();
		void DebugDisplay();

	private:
		// Initialization Functions
		void InitializeGBuffer();
		void InitializeSkydome();
		void InitializeHammersley(unsigned int n);

		void CreateAmbientShader();
		void CreateDeferredShader();
		void CreateLocalLightShader();
		void CreateSkydomeShader();

		void CreateShadowMap(LightData& light);
		void BlurShadowMap(LightData& light);
		std::vector<float> CreateBlurKernel(unsigned int distance);

		void GBufferPass();
		void AmbientPass();
		void GlobalLightingPass(LightData& light);
		void LocalLightingPass();

		void DrawAllRenderData(Shader* pShader);
		void DrawShadowCastingObjects(Shader* pShader);
		void DrawFSQ();
		void DrawSphere();

		void DrawParticles();

		void DrawSkydome();

		void DrawDebugDrawings();

		void DrawShadowMap();

		void ApplyFXAA();

		// ImGui Debug functions
		void DebugDisplayGBuffer();
		void DebugDisplayShadow();
		void DebugDisplayIBL();
		void DebugDisplayAA();

	public:
		std::vector<RenderData> mRenderData;
		std::vector<DebugRenderData> mDebugRenderData;
		std::vector<LightData> mLightData;
		std::vector<ParticleData> mParticleData;
		std::vector<CameraData> mCameraData;

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
		Shader* mpHorizontalBlurShader;
		Shader* mpVerticalBlurShader;

		// ParticleSystem
		Shader* mpParticleShader;
		Shader* mpParticleCompute;
		ShaderStorageBuffer* mpParticlesPositionStorage;
		bool ShowParticles;

		// Post-Processing
		Shader* mpAAShader;
		FrameBuffer* mpFinalBuffer;
		int mFXAA;
		float mFXAASpan;
	};
}