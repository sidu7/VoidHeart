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
#include "Hollow/Managers/AudioManager.h"
#include "Hollow/Managers/EventManager.h"
#include "Hollow/Managers/ThreadManager.h"

#define SOL_ALL_SAFETIES_ON 1
#include <sol/sol.hpp> // or #include "sol.hpp", whichever suits your needs

//#include "Hollow/Graphics/Camera.h"

void apple() {
	std::cout << "apple";
}
namespace Hollow {

#define BIND_EVENT_FN(x) std::bind(&Application::x, this, std::placeholders::_1)
	
	Application::Application(const std::string& settingsFilePath)
	{
		PARSE_JSON_FILE(settingsFilePath);
		
		rapidjson::Value::Object data = root.GetObject();
		mpWindow = new GameWindow(JSONHelper::GetSettings(data,"Window"));

		InputManager::Instance().SetEventCallback(BIND_EVENT_FN(OnEvent));
		
		mIsRunning = true;
		// Initalize managers
		ThreadManager::Instance().Init();
		MemoryManager::Instance().Init(JSONHelper::GetSettings(data,"Memory"));
		RenderManager::Instance().Init(JSONHelper::GetSettings(data, "Renderer"), mpWindow);
		SystemManager::Instance().Init();
		ImGuiManager::Instance().Init(mpWindow);
		ResourceManager::Instance().Init();
        AudioManager::Instance().Init();


		FrameRateController::Instance().SetMaxFrameRate(data["FrameRate"].GetUint());
	}

	Application::~Application()
	{
		ResourceManager::Instance().CleanUp();
		RenderManager::Instance().CleanUp();
		ImGuiManager::Instance().CleanUp();
		ThreadManager::Instance().CleanUp();
		delete mpWindow;
	}

	void Application::OnEvent(Event& e)
	{

		EventDispatcher dispatcher(e);
		dispatcher.Dispatch<WindowCloseEvent>(BIND_EVENT_FN(OnWindowClose));
		
		//RenderManager::Instance().GetCamera()->OnEvent(e);

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
		
		sol::state lua;
		lua.open_libraries(sol::lib::base);

		lua.script("print('bark bark bark!')");

		while (mIsRunning)
		{			
			FrameRateController::Instance().FrameStart();
			// Start frame functions
			ImGuiManager::Instance().StartFrame();

			// Update functions
			for (Layer* layer : mLayerStack)
			{
				layer->OnUpdate(FrameRateController::Instance().GetFrameTime());
			}
			InputManager::Instance().Update();

			SystemManager::Instance().Update();

			EventManager::Instance().HandleEvents();

			AudioManager::Instance().Update();


			RenderManager::Instance().Update();

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
