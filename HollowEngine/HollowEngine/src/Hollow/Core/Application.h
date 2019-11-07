#pragma once
#include "Hollow/Graphics/GameWindow.h"
#include "Layer.h"
#include "LayerStack.h"

namespace Hollow {
	class WindowCloseEvent;
	class Event;

	class RenderManager;

	class Application
	{
		//SINGLETON(Application)
	public:
		HOLLOW_API Application(const std::string& settingsFilePath);
		HOLLOW_API virtual ~Application();
		 
		HOLLOW_API //inline static Application& Instance() { return *instance; }
		HOLLOW_API void OnEvent(Event& e);
		HOLLOW_API void Run();
		 
		HOLLOW_API void PushLayer(Layer* layer);
		HOLLOW_API void PushOverlay(Layer* layer);
		
	private:
		bool OnWindowClose(WindowCloseEvent& e);

		GameWindow* mpWindow;
		bool mIsRunning;
		LayerStack mLayerStack;
		//static Application* instance;
	};

	//To be defined in CLIENT
	Application* CreateApplication();

}
