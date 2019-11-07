#include <hollowpch.h>

#include "GameWindow.h"

namespace Hollow {
	GameWindow::GameWindow(rapidjson::Value::Object& data) : mTitle(data["Name"].GetString()), mWidth(data["Width"].GetInt()), mHeight(data["Height"].GetInt())
	{
		Initialize(data);
	}

	GameWindow::~GameWindow()
	{
		SDL_GL_DeleteContext(mContext);
		SDL_DestroyWindow(mpWindow);
		SDL_Quit();
	}

	void GameWindow::Initialize(rapidjson::Value::Object& data)
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

		//SDL Opengl settings
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, data["OpenglMajorVersion"].GetInt());
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, data["OpenglMinorVersion"].GetInt());
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
		SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

		// Enable AntiAliasiing (Don't know what does this do)
		SDL_GL_SetAttribute(SDL_GL_MULTISAMPLEBUFFERS, 1);
		SDL_GL_SetAttribute(SDL_GL_MULTISAMPLESAMPLES, 4);
		SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);

		Uint32 flags = SDL_WINDOW_OPENGL | SDL_WINDOW_MOUSE_CAPTURE;
		
		// Create SDL window
		mpWindow = SDL_CreateWindow(mTitle.c_str(), SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, mWidth, mHeight, flags);

		if (mpWindow == NULL)
		{
			HW_CORE_ERROR("Could not create window: {0}", SDL_GetError());
		}

		// Create OpenGL context
		mContext = SDL_GL_CreateContext(mpWindow);
		
		// Set this flag if Vysnc is wanted
		SDL_GL_SetSwapInterval(data["VSync"].GetBool());
	}
}
