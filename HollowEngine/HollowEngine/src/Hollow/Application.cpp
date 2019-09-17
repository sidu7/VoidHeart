#include "hollowpch.h"

#include"Application.h"
#include "Managers/SystemManager.h"

namespace Hollow {
	Application::Application()
	{
		mWindow = std::unique_ptr<Window>(Window::Create("Hollow Engine", 1280, 720));
		mIsRunning = true;
	}

	Application::~Application()
	{
	}

	void Application::Run()
	{
		while (true)
		{
			SystemManager::Instance().Update();
		}
	}
}