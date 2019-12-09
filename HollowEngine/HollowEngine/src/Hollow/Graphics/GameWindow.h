#pragma once
#include "Hollow/Core/Core.h"

namespace Hollow {
	class GameWindow
	{
	public:
		/// <summary>
		/// Initializes a new instance of the <see cref="GameWindow"/> class.
		/// </summary>
		/// <param name="data">The data.</param>
		HOLLOW_API GameWindow(rapidjson::Value::Object& data);

		/// <summary>
		/// Destructor for an instance of the <see cref="GameWindow"/> class.
		/// </summary>
		HOLLOW_API ~GameWindow();
		 
		/// <summary>
		/// Gets a pointed to the SDL Window
		/// </summary>
		/// <returns>Pointer to SDL Window</returns>
		HOLLOW_API inline SDL_Window* GetWindow() { return mpWindow; }

		/// <summary>
		/// Gets the SDL OpenGL context
		/// </summary>
		/// <returns>The SDL OpenGL context</returns>
		HOLLOW_API inline SDL_GLContext GetContext() { return mContext; }

		/// <summary>
		/// Gets the width of the window in pixels
		/// </summary>
		/// <returns>Width of the window in pixels</returns>
		HOLLOW_API inline int GetWidth() { return mWidth; }

		/// <summary>
		/// Gets the height of the window in pixels
		/// </summary>
		/// <returns>Height of the window in pixels</returns>
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
