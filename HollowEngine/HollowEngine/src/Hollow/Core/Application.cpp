#include <hollowpch.h>

#include"Application.h"
#include "Hollow/Events/ApplicationEvent.h"

#include "Hollow/Managers/InputManager.h"

#include "Hollow/Managers/SystemManager.h"
#include "Hollow/Managers/RenderManager.h"
#include "Hollow/Managers/MemoryManager.h"

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
