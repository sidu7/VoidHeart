#pragma once

#ifdef HW_PLATFORM_WINDOWS 

extern Hollow::Application* Hollow::CreateApplication();

int main(int argc, char** argv)
{
	//This has to be moved out of the main as well later.
	Hollow::Log::Init();
	HW_CORE_WARN("The log is ready!");
	//Example for how to send a variable to log
	int a = 5;
	HW_INFO("Oi yoi yoi x {0}", a);

	auto app = Hollow::CreateApplication();
	app->Run();
	delete app;

	

	return 0;
}

#endif 