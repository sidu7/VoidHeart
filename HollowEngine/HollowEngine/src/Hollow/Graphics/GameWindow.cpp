#include <hollowpch.h>

#include "GameWindow.h"

namespace Hollow {
	GameWindow::GameWindow(std::string title, int width, int height) : mTitle(title), mWidth(width), mHeight(height)
	{
		Initialize();
	}

	GameWindow::~GameWindow()
	{
	}

	void GameWindow::Initialize()
	{
		HW_CORE_INFO("Creating SDL window");
		static bool initSDL = true;
		// Init SDL if it needs to be
		if (initSDL)
		{
			HW_CORE_INFO("Initalizing SDL");
			SDL_Init(SDL_INIT_VIDEO | SDL_INIT_GAMECONTROLLER);
			initSDL = false;
		}

		// Create SDL window
		mpWindow = SDL_CreateWindow(mTitle.c_str(), SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, mWidth, mHeight, SDL_WINDOW_OPENGL);

		if (mpWindow == NULL)
		{
			HW_CORE_ERROR("Could not create window: {0}", SDL_GetError());
		}

		// Create OpenGL context
		mContext = SDL_GL_CreateContext(mpWindow);
		
		// Set this flag if Vysnc is wanted
		SDL_GL_SetSwapInterval(0);
	}
}
