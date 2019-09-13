#include "hollowpch.h"

#include"Application.h"

namespace Hollow {

	Application::Application()
	{
		mWindow = std::unique_ptr<Window>(Window::Create("Hollow Engine", 1024, 720));
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