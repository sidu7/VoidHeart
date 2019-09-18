#include "hollowpch.h"
#include "Input.h"
#include "Application.h"

#include <SDL2/SDL_keyboard.h>
#include <SDL2/SDL_mouse.h>
#include <SDL2/SDL_gamecontroller.h>
#include "GameWindow.h"

namespace Hollow {

	bool InputManager::IsKeyPressed(int keycode)
	{
		return mCurrentState[keycode];
	}



	bool InputManager::IsMouseButtonPressed(int button)
	{
		return mCurrentMouseState[button];
	}



	std::pair<float, float> InputManager::GetMousePosition()
	{
		auto window = static_cast<SDL_Window*>(Application::Instance().GetWindow().GetWindow());

		double xpos, ypos;

		//glfwGetCursorPos(window, &xpos, &ypos);
		return { (float)xpos, (float)ypos };
	}

	float InputManager::GetMouseX()
	{
		//auto [x, y] = GetMousePosition();
		return 0;// x;
	}



	float InputManager::GetMouseY()
	{
		//auto [x, y] = GetMousePositionImpl();
		return 0;//y;
	}

	void InputManager::Init()
	{
		SDL_memset(mCurrentState, 0, 512 * sizeof(Uint8));
		SDL_memset(mPreviousState, 0, 512 * sizeof(Uint8));
		SDL_memset(mPrevMouseState, 0, 3 * sizeof(bool));
		SDL_memset(mCurrentMouseState, 0, 3 * sizeof(bool));
	}

	void InputManager::Update()
	{
		//fetch keyboard state
		int numberOfFetchedKeys = 0;
		const Uint8* pCurrentKeyStates = SDL_GetKeyboardState(&numberOfFetchedKeys);

		if (numberOfFetchedKeys > 512)
		{
			numberOfFetchedKeys = 512;
		}

		SDL_JoystickEventState(1);
		SDL_memcpy(mPreviousState, mCurrentState, 512 * sizeof(Uint8));
		SDL_memcpy(mCurrentState, pCurrentKeyStates, numberOfFetchedKeys * sizeof(Uint8));
	}



}