#pragma once

#include "Utils/Singleton.h"
#include <GL/glew.h>

namespace Hollow {

	class GameWindow;
	class Camera;
	class Shader;
	class Shape;
	class Material;

	class HOLLOW_API RenderManager
	{
		SINGLETON(RenderManager)
	public:
		void Init(GameWindow* pWindow = nullptr);
		void CleanUp();

		void Update();

		inline Camera* GetCamera(){ return mpCamera; }

	public:
		std::vector<Shape*> mShapes;
		std::vector<Material*> mMaterials;

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