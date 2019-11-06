#include <hollowpch.h>
#include "InputManager.h"

#include "Hollow/Events/ApplicationEvent.h"
#include "Hollow/Events/MouseEvent.h"
#include "Hollow/Events/KeyEvent.h"

#include <SDL2/SDL_keyboard.h>
#include <SDL2/SDL_mouse.h>

namespace Hollow {

	bool InputManager::IsKeyPressed(unsigned int keycode)
	{
		return mCurrentState[keycode];
	}

	bool InputManager::IsKeyReleased(unsigned int keycode)
	{
		return !mCurrentState[keycode] && mPreviousState[keycode];
	}

	bool InputManager::IsKeyTriggered(unsigned int keycode)
	{
		return mCurrentState[keycode] && !mPreviousState[keycode];
	}

	bool InputManager::IsMouseButtonTriggered(unsigned int keycode)
	{
		return mCurrentMouseState[keycode] && !mPrevMouseState[keycode];
	}
	
	bool InputManager::IsMouseButtonPressed(unsigned int button)
	{
		return mCurrentMouseState[button];
	}

	std::pair<float, float> InputManager::GetMouseMotion()
	{
		return std::pair<float, float>(xRel, yRel);
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

	void InputManager::SetEventCallback(const EventCallbackFn& callback)
	{
		EventCallback = callback;
	}

	void InputManager::Init()
	{
		SDL_memset(mCurrentState, 0, 512 * sizeof(Uint8));
		SDL_memset(mPreviousState, 0, 512 * sizeof(Uint8));
		SDL_memset(mPrevMouseState, 0, 6 * sizeof(bool));
		SDL_memset(mCurrentMouseState, 0, 6 * sizeof(bool));
	}

	void InputManager::HandleWindowEvents(const SDL_Event& e)
	{
		if(e.type == SDL_WINDOWEVENT)
		{
			switch(e.window.event)
			{
			case SDL_WINDOWEVENT_RESIZED:
				{
					WindowResizeEvent wre(e.window.data1, e.window.data2);
					EventCallback(wre);
					break;
				}
			case SDL_WINDOWEVENT_CLOSE:
				{
					WindowCloseEvent wce;
					EventCallback(wce);
					break;
				}
			}
		}
	}

	void InputManager::HandleKeyboardEvents(const SDL_Event& e)
	{
		switch (e.type)
		{
		case SDL_KEYDOWN:
		{
			KeyPressedEvent kpe(e.key.keysym.scancode, e.key.repeat);
			EventCallback(kpe);
			break;
		}

		case SDL_KEYUP:
		{
			KeyPressedEvent kpe(e.key.keysym.scancode, e.key.repeat);
			EventCallback(kpe);
			break;
		}
		}
		
	}

	void InputManager::HandleMouseEvents(const SDL_Event & event)
	{
		if (event.type == SDL_MOUSEWHEEL)
		{
			MouseScrolledEvent mse(event.wheel.x, event.wheel.y);
			EventCallback(mse);
		}
		else if (event.type == SDL_MOUSEBUTTONDOWN)
		{
			// ... handle mouse clicks ...
			mCurrentMouseState[event.button.button] = true;

			MouseButtonPressedEvent mpe(event.button.button);
			EventCallback(mpe);

		}
		else if(event.type == SDL_MOUSEBUTTONUP)
		{
			mCurrentMouseState[event.button.button] = false;
			
			MouseButtonReleasedEvent mre(event.button.button);
			EventCallback(mre);
		}
		else if(event.type == SDL_MOUSEMOTION)
		{
			xRel = event.motion.xrel;
			yRel = -event.motion.yrel;
		}
	}

	void InputManager::HideMouseCursor()
	{
		SDL_ShowCursor(0);
		SDL_SetRelativeMouseMode(SDL_TRUE);
	}

	void InputManager::ShowMouseCursor()
	{
		SDL_ShowCursor(1);
		SDL_SetRelativeMouseMode(SDL_FALSE);
	}

	void InputManager::Update()
	{
		SDL_Event e;
		SDL_memcpy(mPrevMouseState, mCurrentMouseState, 6 * sizeof(bool));

		xRel = 0.0f; yRel = 0.0f;
		
		while (SDL_PollEvent(&e) != 0)
		{
			ImGui_ImplSDL2_ProcessEvent(&e);
			
			HandleWindowEvents(e);
			HandleKeyboardEvents(e);
			HandleMouseEvents(e);
			// TODO add controller support here
		}
		
		//fetch keyboard state
		int numberOfFetchedKeys = 0;
		const Uint8* pCurrentKeyStates = SDL_GetKeyboardState(&numberOfFetchedKeys);

		if (numberOfFetchedKeys > 512)
		{
			numberOfFetchedKeys = 512;
		}
		
		SDL_memcpy(mPreviousState, mCurrentState, numberOfFetchedKeys * sizeof(Uint8));
		SDL_memcpy(mCurrentState, pCurrentKeyStates, numberOfFetchedKeys * sizeof(Uint8));
	}



}