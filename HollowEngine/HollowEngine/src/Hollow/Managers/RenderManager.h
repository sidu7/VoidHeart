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

	public:
		std::vector<RenderData> mRenderData;
		std::vector<DebugRenderData> mDebugRenderData;
	private:
		// Initialization Functions
		void InitializeGBuffer();

		void CreateDeferredShader();

		void CreateShadowMap();

		void GBufferPass();
		void GlobalLightingPass();

		void DrawAllRenderData(Shader* pShader);
		void DrawFSQ();

		void DrawDebugDrawings();

		// ImGui Debug functions
		void DebugDisplay();
		void DebugDisplayGBuffer();

	private:
		// Transformation matricies
		glm::mat4 mProjectionMatrix;
		glm::mat4 mViewMatrix;

	private:
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
		FrameBuffer* mpShadowMap;
		glm::mat4 mShadowMatrix;
	};
}