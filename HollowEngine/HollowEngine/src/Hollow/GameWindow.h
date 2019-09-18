#pragma once

#include "Window.h"
#include <SDL2/SDL.h>

namespace Hollow {
	class GameWindow : public Window
	{
	public:
		GameWindow(std::string title = "Hollow Engine", int width = 1024, int height = 720);
		virtual ~GameWindow();

		void* GetWindow() override;

	private:
		virtual void Initialize();

	private:
		std::string mTitle;
		int mWidth;
		int mHeight;
		SDL_Window* mWindow;
	};
}
