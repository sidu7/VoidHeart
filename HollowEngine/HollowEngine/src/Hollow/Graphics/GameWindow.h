#pragma once

namespace Hollow {
	class GameWindow
	{
	public:
		HOLLOW_API GameWindow(rapidjson::Value::Object& data);
		HOLLOW_API ~GameWindow();
		 
		HOLLOW_API inline SDL_Window* GetWindow() { return mpWindow; }
		HOLLOW_API inline SDL_GLContext GetContext() { return mContext; }
		HOLLOW_API inline int GetWidth() { return mWidth; }
		HOLLOW_API inline int GetHeight() { return mHeight; }

	private:
		void Initialize(rapidjson::Value::Object& data);

	private:
		SDL_Window* mpWindow;
		SDL_GLContext mContext;
		std::string mTitle;
		int mWidth;
		int mHeight;
	};
}
