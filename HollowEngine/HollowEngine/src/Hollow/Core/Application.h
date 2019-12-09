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
		/// <summary>
		/// Initializes a new instance of the <see cref="Application"/> class.
		/// </summary>
		HOLLOW_API Application() {}

		/// <summary>
		/// Destructor of the instance of the <see cref="Application"/> class.
		/// </summary>
		HOLLOW_API virtual ~Application();

		/// <summary>
		/// Reads the Setting file to initialize various systems and Game Data
		/// </summary>
		/// <param name="settingsFilePath">The Settings file path.</param>
		HOLLOW_API void Init(const std::string& settingsFilePath);
		
		/// <summary>
		/// When called, the event is sent to the dispatcher so that the event can be given to all the layers.
		/// Not fully implemented.
		/// </summary>
		/// <param name="e">The Event.</param>
		HOLLOW_API void OnEvent(Event& e);

		/// <summary>
		/// Runs the application in the Hollow Engine.
		/// </summary>
		HOLLOW_API void Run();
		 
		/// <summary>
		/// Not yet implemented
		/// </summary>
		/// <param name="layer">The layer pointer.</param>
		HOLLOW_API void PushLayer(Layer* layer);

		/// <summary>
		/// Not yet implemented
		/// </summary>
		/// <param name="layer">The layer pointer.</param>
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
