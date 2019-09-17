#include "hollowpch.h"

#include"Application.h"
#include "Events/ApplicationEvent.h"

namespace Hollow {

	Application::Application()
	{
		mWindow = std::unique_ptr<Window>(Window::Create("Hollow Engine", 1280, 720));
		WindowResizeEvent re(1280, 720);
		HW_TRACE(re);

		mIsRunning = true;
	}

	Application::~Application()
	{
	}

	void Application::Run()
	{
		while (true)
		{
		}
	}
}