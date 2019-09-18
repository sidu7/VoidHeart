#pragma once
#include "Core.h"
#include "GameWindow.h"

namespace Hollow {

	class RenderManager;

	class HOLLOW_API Application
	{
	public:
		Application();
		virtual ~Application();

		void Run();

		inline GameWindow* GetWindow() { return mpWindow; }

	private:
		GameWindow* mpWindow;
		bool mIsRunning;
		RenderManager* mpRenderManager;
	};

	//To be defined in CLIENT
	Application* CreateApplication();

}