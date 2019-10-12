#pragma once

#include "Utils/Singleton.h"
#include "Hollow/Graphics/RenderData.h"
#include <GL/glew.h>

namespace Hollow {

	class GameWindow;
	class Camera;
	class Shader;
	class FrameBuffer;

	class HOLLOW_API RenderManager
	{
		SINGLETON(RenderManager)
	public:
		void Init(GameWindow* pWindow = nullptr);
		void CleanUp();
		void Update();
		inline Camera* GetCamera(){ return mpCamera; }

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

		void DrawDebugDrawings();

		// ImGui Debug functions
		void DebugDisplay();
		void DebugDisplayGBuffer();

	public:
		std::vector<RenderData> mRenderData;
		std::vector<DebugRenderData> mDebugRenderData;
		std::vector<LightData> mLightData;
		std::vector<ParticleData> mParticleData;

	private:
		// Transformation matricies
		glm::mat4 mProjectionMatrix;
		glm::mat4 mViewMatrix;

		GameWindow* mpWindow;
		Camera* mpCamera;

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
	};
}