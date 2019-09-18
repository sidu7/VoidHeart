#include "hollowpch.h"
#include "InputManager.h"
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

	bool InputManager::IsKeyTriggered(int keycode)
	{
		return mCurrentState[keycode] && !mPreviousState[keycode];
	}

	bool InputManager::IsMouseButtonPressed(int button)
	{
		return mCurrentMouseState[button];
	}



	std::pair<float, float> InputManager::GetMousePosition()
	{
		int xpos, ypos;
		SDL_GetMouseState(&xpos, &ypos);
		
		return { (float)xpos, (float)ypos };
	}

	float InputManager::GetMouseX()
	{
		auto pos = GetMousePosition();
		return pos.first;
	}



	float InputManager::GetMouseY()
	{
		auto pos = GetMousePosition();
		return pos.second;
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
		SDL_Event e;
		while (SDL_PollEvent(&e) != 0)
		{

			if (e.type == SDL_MOUSEBUTTONDOWN)
			{
				mCurrentMouseState[e.button.button - 1] = true;
			}
			if (e.type == SDL_MOUSEBUTTONUP)
			{
				mCurrentMouseState[e.button.button - 1] = false;
			}
		}
		
		//fetch keyboard state
		int numberOfFetchedKeys = 0;
		const Uint8* pCurrentKeyStates = SDL_GetKeyboardState(&numberOfFetchedKeys);

		if (numberOfFetchedKeys > 512)
		{
			numberOfFetchedKeys = 512;
		}
		
		SDL_memcpy(mPreviousState, mCurrentState, 512 * sizeof(Uint8));
		SDL_memcpy(mCurrentState, pCurrentKeyStates, numberOfFetchedKeys * sizeof(Uint8));
		SDL_memcpy(mPrevMouseState, mCurrentMouseState, 3 * sizeof(bool));

		
		if(IsKeyPressed(SDL_SCANCODE_A))
		{
			HW_TRACE("A Pressed");
		}
	}



}