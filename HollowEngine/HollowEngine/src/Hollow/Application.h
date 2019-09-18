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

		inline static Application& Instance() { return *instance; }

		void Run();

		inline GameWindow& GetWindow() { return *mpWindow; }

	private:
		GameWindow* mpWindow;
		bool mIsRunning;

		static Application* instance;
		RenderManager* mpRenderManager;
	};

	//To be defined in CLIENT
	Application* CreateApplication();

}