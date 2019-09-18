#pragma once

#include "Core.h"

#include "Window.h"

namespace Hollow {

	class HOLLOW_API Application
	{
	public:
		Application();
		virtual ~Application();

		inline static Application& Instance() { return *instance; }

		void Run();
		Window& GetWindow();

	private:
		std::unique_ptr<Window> mWindow;
		bool mIsRunning;

		static Application* instance;
	};

	//To be defined in CLIENT
	Application* CreateApplication();

}