#include "hollowpch.h"

#include"Application.h"
#include "Events/ApplicationEvent.h"
#include "../Utils/Test.h"

namespace Hollow {
	Application* Application::instance = nullptr;

	Application::Application()
	{
		instance = this;

		mWindow = std::unique_ptr<Window>(Window::Create("Hollow Engine", 1280, 720));
		WindowResizeEvent re(1280, 720);
		
		HW_TRACE(re);

		mIsRunning = true;
	}

	Application::~Application()
	{
	}

	Window& Application::GetWindow()
	{
		return *mWindow;
	}

	void Application::Run()
	{
		Test::Instance().work();
		
		while (true)
		{
		}
	}
}
