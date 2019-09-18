#include <hollowpch.h>

#include"Application.h"

#include "Managers/SystemManager.h"
#include "Managers/RenderManager.h"

#include "Log.h"
namespace Hollow {
	Application::Application()
	{
		mpWindow = new GameWindow("Hollow Engine", 1280, 720);
		mIsRunning = true;
		// Initalize managers
		mpRenderManager = new RenderManager(mpWindow);
	}

	Application::~Application()
	{
	}

	void Application::Run()
	{
		
		while (true)
		{
			HW_CORE_WARN("TEST");
			SystemManager::Instance().Update();
			//SystemManager::Instance().Update();
			mpRenderManager->Update();
		}
	}
}