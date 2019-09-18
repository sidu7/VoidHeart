#include "hollowpch.h"

#include "GameWindow.h"

#include <SDL2/SDL.h>


namespace Hollow {

	Window* Window::Create(std::string title, int width, int height)
	{
		return new GameWindow(title, width, height);
	}

	GameWindow::GameWindow(std::string title, int width, int height) : mTitle(title), mWidth(width), mHeight(height)
	{
		Initialize();
	}

	GameWindow::~GameWindow()
	{
	}

	void* GameWindow::GetWindow()
	{
		return mWindow;
	}

	void GameWindow::Initialize()
	{
		std::cout << "Creating SDL window" << std::endl;

		// Init SDL if it needs to be
		if (true)
		{
			std::cout << "Initalizing SDL" << std::endl;
			SDL_Init(SDL_INIT_VIDEO);
		}

		// Create SDL window
		mWindow = SDL_CreateWindow(mTitle.c_str(), SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, mWidth, mHeight, SDL_WINDOW_OPENGL);

		if (mWindow == NULL)
		{
			std::cout << "Could not create window: " << SDL_GetError();
		}

		SDL_GLContext context = SDL_GL_CreateContext(mWindow);

		//glewInit();
		//if (!GLEW_VERSION_2_0)
		{
			//std::cout << "Needs OpenGL version 2.0 or better" << std::endl;
			//return 1;
		}
	
	}
}
