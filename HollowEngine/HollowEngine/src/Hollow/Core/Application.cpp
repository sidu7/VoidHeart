#include <hollowpch.h>

#include"Application.h"
#include "Hollow/Events/ApplicationEvent.h"

#include "Hollow/Managers/InputManager.h"
#include "Hollow/Managers/SystemManager.h"
#include "Hollow/Managers/RenderManager.h"
#include "Hollow/Managers/MemoryManager.h"
#include "Hollow/Managers/ImGuiManager.h"
#include "Hollow/Managers/FrameRateController.h"
#include "Hollow/Managers/ResourceManager.h"


#include "Hollow/Graphics/Camera.h"

namespace Hollow {

#define BIND_EVENT_FN(x) std::bind(&Application::x, this, std::placeholders::_1)
	
	Application::Application()
	{
		//instance = this;
		mpWindow = new GameWindow("Hollow Engine", 1280, 720);

		InputManager::Instance().SetEventCallback(BIND_EVENT_FN(OnEvent));
		
		mIsRunning = true;
		// Initalize managers
		MemoryManager::Instance().Init();
		RenderManager::Instance().Init(mpWindow);
		SystemManager::Instance().Init();
		ImGuiManager::Instance().Init(mpWindow);
		ResourceManager::Instance().Init();


		FrameRateController::Instance().SetMaxFrameRate(60);
	}

	Application::~Application()
	{
		RenderManager::Instance().CleanUp();
		ImGuiManager::Instance().CleanUp();
		delete mpWindow;
	}

	void Application::OnEvent(Event& e)
	{

		EventDispatcher dispatcher(e);
		dispatcher.Dispatch<WindowCloseEvent>(BIND_EVENT_FN(OnWindowClose));
		
		RenderManager::Instance().GetCamera()->OnEvent(e);

		//HW_CORE_TRACE("{0}", e);

		for(auto it = mLayerStack.end(); it != mLayerStack.begin();)
		{
			(*--it)->OnEvent(e);

			if (e.Handled)
				break;
		}
	}

	void Application::Run()
	{
		
		while (mIsRunning)
		{
			FrameRateController::Instance().FrameStart();
			// Start frame functions
			ImGuiManager::Instance().StartFrame();

			// Update functions
			for(Layer* layer : mLayerStack)
			{
				layer->OnUpdate(FrameRateController::Instance().GetFrameTime());
			}
			SystemManager::Instance().Update();
			RenderManager::Instance().GetCamera()->OnUpdate(FrameRateController::Instance().GetFrameTime());
			RenderManager::Instance().Update();
			InputManager::Instance().Update();

			FrameRateController::Instance().FrameEnd();
		}
	}

	void Application::PushLayer(Layer* layer)
	{
		mLayerStack.PushLayer(layer);
	}

	void Application::PushOverlay(Layer* layer)
	{
		mLayerStack.PushOverlay(layer);
	}

	bool Application::OnWindowClose(WindowCloseEvent& e)
	{
		mIsRunning = false;
		return true;
	}
}
