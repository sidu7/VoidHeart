#include <hollowpch.h>

#include"Application.h"
#include "Hollow/Events/ApplicationEvent.h"

#include "Hollow/Managers/InputManager.h"

#include "Hollow/Managers/SystemManager.h"
#include "Hollow/Managers/RenderManager.h"
#include "Hollow/Managers/MemoryManager.h"

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

		// TESTING BELOW -----------------
		GameObject* pGameObject = new GameObject();
		Shape* pShape = new Shape();
		pShape->Init();
		pGameObject->AddComponent(pShape);

		RenderManager::Instance().mShapes.push_back(pShape);
	}

	Application::~Application()
	{
	}

	void Application::OnEvent(Event& e)
	{

		EventDispatcher dispatcher(e);
		dispatcher.Dispatch<WindowCloseEvent>(BIND_EVENT_FN(OnWindowClose));
		
		HW_CORE_TRACE("{0}", e);

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

			for(Layer* layer : mLayerStack)
			{
				layer->OnUpdate(0.016f);
			}
			SystemManager::Instance().Update();
			RenderManager::Instance().Update();
			InputManager::Instance().Update();
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
