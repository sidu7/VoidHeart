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
		void Init(GameWindow* pWindow = nullptr);
		void CleanUp();
		void Update();
		inline glm::vec2 GetWindowSize();

	private:
		// Initialization Functions
		void InitializeGBuffer();

		void CreateDeferredShader();

		void CreateShadowMap(LightData& light);

		void GBufferPass();
		void GlobalLightingPass(LightData& light);

		void DrawAllRenderData(Shader* pShader);
		void DrawShadowCastingObjects(Shader* pShader);
		void DrawFSQ();

		void DrawParticles();

		void DrawDebugDrawings();

		// ImGui Debug functions
		void DebugDisplay();
		void DebugDisplayGBuffer();

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

		// Debug drawing Shader
		Shader* mpDebugShader;

		// G-Buffer
		Shader* mpGBufferShader;
		FrameBuffer* mpGBuffer;
		int mGBufferDisplayMode;

		// Shadows
		Shader* mpShadowMapShader;

		// ParticleSystem
		Shader* mpParticleShader;
		Shader* mpParticleCompute;
		ShaderStorageBuffer* mpParticlesPositionStorage;
		bool ShowParticles;
	};
}