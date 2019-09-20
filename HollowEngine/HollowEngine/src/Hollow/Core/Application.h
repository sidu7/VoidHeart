#pragma once
#include "Hollow/Graphics/GameWindow.h"

namespace Hollow {

	class RenderManager;

	class HOLLOW_API Application
	{
		//SINGLETON(Application)
	public:
		Application();
		virtual ~Application();

		//inline static Application& Instance() { return *instance; }

		void Run();

	private:
		GameWindow* mpWindow;
		bool mIsRunning;

		//static Application* instance;
	};

	//To be defined in CLIENT
	Application* CreateApplication();

}