#include"Application.h"
#include "../Utils/Test.h"



namespace Hollow {

	Application::Application()
	{

	}

	Application::~Application()
	{

	}

	void Application::Run()
	{
		Test::Instance().work();
		
		while (true);
	}
}