#pragma once

#include "Window.h"

namespace Hollow {
	class GameWindow : public Window
	{
	public:
		GameWindow(std::string title = "Hollow Engine", int width = 1024, int height = 720);
		virtual ~GameWindow();

	private:
		virtual void Initialize();

	private:
		std::string mTitle;
		int mWidth;
		int mHeight;
	};
}