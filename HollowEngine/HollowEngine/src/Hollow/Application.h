#pragma once

#include "Core.h"

#include "Window.h"

namespace Hollow {

	class HOLLOW_API Application
	{
	public:
		Application();
		virtual ~Application();

		void Run();

	private:
		std::unique_ptr<Window> mWindow;
		bool mIsRunning;
	};

	//To be defined in CLIENT
	Application* CreateApplication();

}