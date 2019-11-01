#pragma once

#include "Utils/Singleton.h"
#include "Hollow/Graphics/Data/RenderData.h"
#include "Hollow/Graphics/Data/LightData.h"
#include "Hollow/Graphics/Data/MaterialData.h"
#include "Hollow/Graphics/Data/DebugRenderData.h"
#include "Hollow/Graphics/Data/CameraData.h"
#include "Hollow/Graphics/Data/ParticleData.h"
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
		void Init(rapidjson::Value::Object& data, GameWindow* pWindow = nullptr);
		void CleanUp();
		void Update();
		inline glm::vec2 GetWindowSize();

	private:
		// Initialization Functions
		void InitializeGBuffer(rapidjson::Value::Object& data);

		void CreateLocalLightShader();
		void CreateDeferredShader(rapidjson::Value::Object& data);

		void CreateShadowMap(LightData& light);
		void BlurShadowMap(LightData& light);
		std::vector<float> CreateBlurKernel(unsigned int distance);

		void GBufferPass();
		void GlobalLightingPass(LightData& light);
		void LocalLightingPass();

		void DrawAllRenderData(Shader* pShader);
		void DrawShadowCastingObjects(Shader* pShader);
		void DrawFSQ();
		void DrawSphere();

		void DrawParticles();

		void DrawDebugDrawings();

		void DrawShadowMap();

		// ImGui Debug functions
		void DebugDisplay();
		void DebugDisplayGBuffer();
		void DebugDisplayLighting();

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

		// Lighting
		Shader* mpDeferredShader;
		Shader* mpLocalLightShader;

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
		unsigned int mShadowMapDebugLightIndex;

		// Blur
		Shader* mpHorizontalBlurShader;
		Shader* mpVerticalBlurShader;

		// ParticleSystem
		Shader* mpParticleShader;
		Shader* mpParticleCompute;
		ShaderStorageBuffer* mpParticlesPositionStorage;
		bool ShowParticles;
	};
}