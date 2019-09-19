#include <hollowpch.h>

#include"Application.h"
#include "Events/ApplicationEvent.h"

#include "Managers/InputManager.h"

#include "Managers/SystemManager.h"
#include "Managers/RenderManager.h"
#include "Managers/MemoryManager.h"

#include "Log.h"
namespace Hollow {
	Application::Application()
	{
		//instance = this;
		mpWindow = new GameWindow("Hollow Engine", 1280, 720);
		
		WindowResizeEvent re(1280, 720);
		
		HW_TRACE(re);

		mIsRunning = true;
		// Initalize managers
		MemoryManager::Instance().Init();
		RenderManager::Instance().Init(mpWindow);
	}

	Application::~Application()
	{
	}

	void Application::Run()
	{
		
		while (true)
		{
			SystemManager::Instance().Update();
			RenderManager::Instance().Update();
			InputManager::Instance().Update();
		}
	}
}
