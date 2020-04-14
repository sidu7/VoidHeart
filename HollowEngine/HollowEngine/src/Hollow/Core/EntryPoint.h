#pragma once

#ifdef HW_PLATFORM_WINDOWS 

extern Hollow::Application* Hollow::CreateApplication();
#undef main

//int main(int argc, char** argv)
//int WINAPI WinMain(
//	HINSTANCE hInstance,
//	HINSTANCE hPrevInstance,
//	LPSTR lpCmdLine,
//	int nCmdShow)
int main(int argc, char** argv)
{
	
	//This has to be moved out of the main as well later.
	Hollow::Log::Init();
	//HW_CORE_WARN("The log is ready!");
	//Example for how to send a variable to log
	//HW_INFO("Welcome to HOLLOW Engine");

	auto app = Hollow::CreateApplication();
	app->Run();
	delete app;

	

	return 0;
}

#endif 