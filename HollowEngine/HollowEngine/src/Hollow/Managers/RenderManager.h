#pragma once

#include "Utils/Singleton.h"
#include "Hollow/Graphics/Data/RenderData.h"
#include "Hollow/Graphics/Data/LightData.h"
#include "Hollow/Graphics/Data/MaterialData.h"
#include "Hollow/Graphics/Data/DebugRenderData.h"
#include "Hollow/Graphics/Data/CameraData.h"
#include "Hollow/Graphics/Data/ParticleData.h"
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

	private:
		// Initialization Functions
		void InitializeGBuffer(rapidjson::Value::Object& data);

		void CreateLocalLightShader(rapidjson::Value::Object& data);
		void CreateDeferredShader(rapidjson::Value::Object& data);

		void CreateShadowMap(LightData& light);
		void BlurTexture(unsigned int inputTextureID, unsigned int width, unsigned int height, unsigned int channels, unsigned int blurWidth, unsigned int& outputTextureID);
		std::vector<float> CreateBlurKernel(unsigned int distance);

		void GBufferPass(CameraData& cameraData);
		void GlobalLightingPass(LightData& light, glm::vec3 eyePosition);
		void LocalLightingPass();

		void DrawAllRenderData(Shader* pShader);
		void DrawShadowCastingObjects(Shader* pShader);
		void DrawFSQ();
		void DrawSphere();

		void DrawParticles(CameraData& cameraData);

		void DrawDebugDrawings();

		void DrawShadowMap();

		void DrawSceneWithBloom();

		void DrawUI();

		// ImGui Debug functions
		void DebugDisplay();
		void DebugDisplayGBuffer();
		void DebugDisplayLighting();

	public:
		std::vector<RenderData> mRenderData;
		std::vector<DebugRenderData> mDebugRenderData;
		std::vector<LightData> mLightData;
		std::vector<ParticleData> mParticleData;
		std::vector<DebugPathData> mDebugPathData;
		std::vector<UIRenderData> mUIData;
		
		CameraData mMainCamera;
		CameraData mUICamera;
		std::vector<CameraData> mSecondaryCameras;

	private:

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
		UniformBuffer* mpWeights;
		Shader* mpHorizontalBlurShader;
		Shader* mpVerticalBlurShader;

		// ParticleSystem
		Shader* mpParticleShader;
		ShaderStorageBuffer* mpParticlesPositionStorage;
		bool ShowParticles;

		// Bloom
		Shader* mpBloomShader;
		FrameBuffer* mpBloomFrame;
		bool mBloomEnabled;

		// UI Shader
		Shader* mpUIShader;
	};
}