#pragma once

#include "Utils/Singleton.h"
#include "Hollow/Components/Shape.h"
#include <GL/glew.h>

namespace Hollow {

	class GameWindow;
	class Camera;
	class Shader;

	class RenderManager
	{
		SINGLETON(RenderManager)
	public:
		~RenderManager();

		void Init(GameWindow* pWindow = nullptr);

		void Update();

		inline Camera* GetCamera(){ return mpCamera; }

	public:
		std::vector<Shape*> mShapes;

	private:
		// Transformation matricies
		glm::mat4 mProjectionMatrix;
		glm::mat4 mViewMatrix;

	private:
		GameWindow* mpWindow;
		Camera* mpCamera;
		Shader* mpTestShader;
	};
}