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
		Application();
		virtual ~Application();

		//inline static Application& Instance() { return *instance; }
		void OnEvent(Event& e);
		void Run();

		void PushLayer(Layer* layer);
		void PushOverlay(Layer* layer);
		
	private:
		bool OnWindowClose(WindowCloseEvent& e);
		void ThreadLoop();

		GameWindow* mpWindow;
		bool mIsRunning;
		bool shouldGoIn = true;
		bool shoudlMainThreadSleep = true;
		LayerStack mLayerStack;
		//static Application* instance;
	};

	//To be defined in CLIENT
	Application* CreateApplication();

}
