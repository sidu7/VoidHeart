#pragma once

namespace Hollow {
	class HOLLOW_API GameWindow
	{
	public:
		GameWindow(std::string title = "Hollow Engine", int width = 1024, int height = 720);
		~GameWindow();

		inline SDL_Window* GetWindow() { return mpWindow; }
		inline SDL_GLContext GetContext() { return mContext; }

	private:
		void Initialize();

	private:
		SDL_Window* mpWindow;
		SDL_GLContext mContext;
		std::string mTitle;
		int mWidth;
		int mHeight;
	};
}
