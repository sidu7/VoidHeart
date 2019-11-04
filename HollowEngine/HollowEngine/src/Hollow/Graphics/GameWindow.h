#pragma once

namespace Hollow {
	class HOLLOW_API GameWindow
	{
	public:
		GameWindow(rapidjson::Value::Object& data);
		~GameWindow();

		inline SDL_Window* GetWindow() { return mpWindow; }
		inline SDL_GLContext GetContext() { return mContext; }
		inline int GetWidth() { return mWidth; }
		inline int GetHeight() { return mHeight; }

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
