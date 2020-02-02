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
#include "Hollow/Managers/ScriptingManager.h"
#include "Hollow/Managers/UIManager.h"
#include "Hollow/Managers/PhysicsManager.h"
#include "Hollow/Managers/SceneManager.h"
#include "Hollow/Managers/GameObjectManager.h"
#include "Hollow/Managers/LocalizationManager.h"


namespace Hollow {


#define BIND_EVENT_FN(x) std::bind(&Application::x, this, std::placeholders::_1)

	Application::~Application()
	{
		ResourceManager::Instance().CleanUp();
		EventManager::Instance().CleanUp();
		RenderManager::Instance().CleanUp();
		ImGuiManager::Instance().CleanUp();
		ThreadManager::Instance().CleanUp();
		UIManager::Instance().CleanUp();
		SystemManager::Instance().CleanUp();
		SceneManager::Instance().CleanUp();
		MemoryManager::Instance().CleanUp();
		delete mpWindow;
	}

	void Application::Init(const std::string& settingsFilePath)
	{

		std::ifstream file(settingsFilePath);
		std::string contents((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
		file.close();
		PARSE_JSON_FILE(contents.c_str());

		rapidjson::Value::Object data = root.GetObject();
		mpWindow = new GameWindow(JSONHelper::GetSettings(data, "Window"));

		InputManager::Instance().SetEventCallback(BIND_EVENT_FN(OnEvent));

		mIsRunning = true;

		// Initalize managers
		InputManager::Instance().Init();
		ThreadManager::Instance().Init();
		MemoryManager::Instance().Init(JSONHelper::GetSettings(data, "Memory"));
		RenderManager::Instance().Init(JSONHelper::GetSettings(data, "Renderer"), mpWindow);
		ScriptingManager::Instance().Init(JSONHelper::GetSettings(data, "Scripting"));
		SystemManager::Instance().Init();
		ImGuiManager::Instance().Init(mpWindow);
		LocalizationManager::Instance().Init(JSONHelper::GetSettings(data, "Localization"));
		EventManager::Instance().Init(JSONHelper::GetSettings(data, "Events"));
		ResourceManager::Instance().Init();
		AudioManager::Instance().Init();
		UIManager::Instance().Init();
		PhysicsManager::Instance().Init(JSONHelper::GetSettings(data, "Physics"));
		SceneManager::Instance().Init();


		FrameRateController::Instance().SetMaxFrameRate(data["FrameRate"].GetUint());
	}

	void Application::OnEvent(Event& e)
	{

		EventDispatcher dispatcher(e);
		dispatcher.Dispatch<WindowCloseEvent>(BIND_EVENT_FN(OnWindowClose));
		//RenderManager::Instance().GetCamera()->OnEvent(e);

		//HW_CORE_TRACE("{0}", e);

		for (auto it = mLayerStack.end(); it != mLayerStack.begin();)
		{
			(*--it)->OnEvent(e);

			if (e.Handled)
				break;
		}
	}

	void Application::Run()
	{
#ifdef _DEBUG
		return;
#else
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

			EventManager::Instance().Update();

			SystemManager::Instance().Update();

			AudioManager::Instance().Update();

			RenderManager::Instance().Update();

			GameObjectManager::Instance().ClearDeletionList();

			FrameRateController::Instance().FrameEnd();

			if (InputManager::Instance().IsKeyPressed("Escape"))
				mIsRunning = false;
		}
#endif	
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
