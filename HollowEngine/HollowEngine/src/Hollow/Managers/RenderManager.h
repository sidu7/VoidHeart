#pragma once

namespace Hollow {

	class GameWindow;
	class Camera;
	class Shader;

	class RenderManager
	{
	public:
		RenderManager(GameWindow* pWindow = nullptr);
		~RenderManager();

		void Init(GameWindow* pWindow);

		void Update();

		inline Camera* GetCamera(){ return mpCamera; }

	private:
		void MakeVAO();
		unsigned int mVAO;
		unsigned int mVBO;
		unsigned int mEBO;
		// Transformation matricies
		glm::mat4 mProjectionMatrix;
		glm::mat4 mViewMatrix;

	private:
		GameWindow* mpWindow;
		Camera* mpCamera;
		Shader* mpTestShader;
	};
}