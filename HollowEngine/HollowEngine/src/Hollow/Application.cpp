#include <hollowpch.h>

#include"Application.h"
#include "Events/ApplicationEvent.h"
#include "../Utils/Test.h"
#include "InputManager.h"

#include "Managers/SystemManager.h"
#include "Managers/RenderManager.h"

#include "Log.h"
namespace Hollow {
	Application::Application()
	{
		mpWindow = new GameWindow("Hollow Engine", 1280, 720);
		instance = this;
		
		WindowResizeEvent re(1280, 720);
		
		HW_TRACE(re);

		mIsRunning = true;
		// Initalize managers
		mpRenderManager = new RenderManager(mpWindow);
	}

	Application::~Application()
	{
	}

	Window& Application::GetWindow()
	{
		return *mpWindow;
	}

	void Application::Run()
	{
		
		while (true)
		{
			HW_CORE_WARN("TEST");
			SystemManager::Instance().Update();
			//SystemManager::Instance().Update();
			mpRenderManager->Update();
		InputManager::Instance().Update();
		}
	}
}
