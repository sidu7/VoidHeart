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

	private:
		// Initialization Functions
		void InitializeGBuffer();

		void CreateDeferredShader();

		void GBufferPass();
		void GlobalLightingPass();

		void DrawAllRenderData(Shader* pShader);
		void DrawFSQ();

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

		// G-Buffer
		Shader* mpGBufferShader;
		FrameBuffer* mpGBuffer;
		int mGBufferDisplayMode;
	};
}