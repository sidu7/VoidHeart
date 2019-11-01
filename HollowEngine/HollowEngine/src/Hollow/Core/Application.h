#pragma once
#include "Hollow/Graphics/GameWindow.h"
#include "Layer.h"
#include "LayerStack.h"

namespace Hollow {
	class WindowCloseEvent;
	class Event;

	class RenderManager;

	class HOLLOW_API Application
	{
		//SINGLETON(Application)
	public:
		Application(const std::string& settingsFilePath);
		virtual ~Application();

		//inline static Application& Instance() { return *instance; }
		void OnEvent(Event& e);
		void Run();

		void PushLayer(Layer* layer);
		void PushOverlay(Layer* layer);
		
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
