#include <hollowpch.h>
#include "InputManager.h"

#include "Hollow/Events/ApplicationEvent.h"
#include "Hollow/Events/MouseEvent.h"
#include "Hollow/Events/KeyEvent.h"

#include <SDL2/SDL_keyboard.h>
#include <SDL2/SDL_mouse.h>

namespace Hollow {

	bool InputManager::IsKeyPressed(const std::string& keyName)
	{
		auto key = SDL_GetKeyFromName(keyName.c_str());
		if (key == SDLK_UNKNOWN)
		{
			HW_CORE_ERROR("Unknown Key Name");
			return false;
		}

		return mCurrentState[SDL_GetScancodeFromKey(key)];
	}

	bool InputManager::IsKeyReleased(const std::string& keyName)
	{
		auto key = SDL_GetKeyFromName(keyName.c_str());
		if (key == SDLK_UNKNOWN)
		{
			HW_CORE_ERROR("Unknown Key Name");
			return false;
		}

		return !mCurrentState[SDL_GetScancodeFromKey(key)] && mPreviousState[SDL_GetScancodeFromKey(key)];
	}

	bool InputManager::IsKeyTriggered(const std::string& keyName)
	{
		auto key = SDL_GetKeyFromName(keyName.c_str());
		if (key == SDLK_UNKNOWN)
		{
			HW_CORE_ERROR("Unknown Key Name");
			return false;
		}

		return mCurrentState[SDL_GetScancodeFromKey(key)] && !mPreviousState[SDL_GetScancodeFromKey(key)];
	}

	bool InputManager::IsMouseButtonTriggered(unsigned int keycode)
	{
		return mCurrentMouseState[keycode] && !mPrevMouseState[keycode];
	}

	bool InputManager::IsControllerButtonPressed(SDL_GameControllerButton button)
	{
		return mCurrentControllerState[button] && mPrevControllerState[button];
	}

	bool InputManager::IsControllerButtonTriggered(SDL_GameControllerButton button)
	{
		return mCurrentControllerState[button] && !mPrevControllerState[button];
	}

	bool InputManager::IsControllerButtonReleased(SDL_GameControllerButton button)
	{
		return !mCurrentControllerState[button] && mPrevControllerState[button];
	}

	bool InputManager::IsControllerTriggerPressed(SDL_GameControllerAxis axis)
	{
		return mCurrentTriggerState[axis] && mPrevTriggerState[axis];
	}

	bool InputManager::IsControllerTriggerTriggered(SDL_GameControllerAxis axis)
	{
		return mCurrentTriggerState[axis] && !mPrevTriggerState[axis];
	}

	bool InputManager::IsControllerTriggerReleased(SDL_GameControllerAxis axis)
	{
		return !mCurrentTriggerState[axis] && mPrevTriggerState[axis];
	}

	Sint16 InputManager::GetAxisValue(SDL_GameControllerAxis axis)
	{
		return SDL_GameControllerGetAxis(mpController, axis);
	}

	bool InputManager::IsMouseButtonPressed(unsigned int button)
	{
		return mCurrentMouseState[button];
	}

	glm::vec2 InputManager::GetMouseMotion()
	{
		return glm::vec2(xRel, yRel);
	}


	glm::ivec2 InputManager::GetMousePosition()
	{
		int xpos, ypos;
		SDL_GetMouseState(&xpos, &ypos);

		return glm::ivec2(xpos, ypos);
	}

	float InputManager::GetMouseX()
	{
		glm::ivec2 pos = GetMousePosition();
		return static_cast<float>(pos.x);
	}

	float InputManager::GetMouseY()
	{
		glm::ivec2 pos = GetMousePosition();
		return static_cast<float>(pos.y);
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
		SDL_memset(mCurrentControllerState, 0, 15 * sizeof(bool));
		SDL_memset(mPrevControllerState, 0, 15 * sizeof(bool));
		mpController = SDL_GameControllerOpen(0);
	}

	void InputManager::HandleWindowEvents(const SDL_Event& e)
	{
		if (e.type == SDL_WINDOWEVENT)
		{
			switch (e.window.event)
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

	void InputManager::HandleControllerEvents(const SDL_Event& e)
	{
		switch (e.type)
		{
		case SDL_CONTROLLERBUTTONDOWN:
		{
			mCurrentControllerState[e.cbutton.button] = true;
			break;
		}
		case SDL_CONTROLLERBUTTONUP:
		{
			mCurrentControllerState[e.cbutton.button] = false;
			break;
		}
		case SDL_CONTROLLERAXISMOTION:
		{
			// TODO: Only works on triggers for the moment, need to abs
			if (e.caxis.value > 3000)
			{
				mCurrentTriggerState[e.caxis.axis] = true;
			}
			else
			{
				mCurrentTriggerState[e.caxis.axis] = false;
			}
			break;
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

	void InputManager::HandleMouseEvents(const SDL_Event& event)
	{
		if (event.type == SDL_MOUSEWHEEL)
		{
			MouseScrolledEvent mse(static_cast<float>(event.wheel.x), static_cast<float>(event.wheel.y));
			EventCallback(mse);
		}
		else if (event.type == SDL_MOUSEBUTTONDOWN)
		{
			// ... handle mouse clicks ...
			mCurrentMouseState[event.button.button] = true;

			MouseButtonPressedEvent mpe(event.button.button);
			EventCallback(mpe);

		}
		else if (event.type == SDL_MOUSEBUTTONUP)
		{
			mCurrentMouseState[event.button.button] = false;

			MouseButtonReleasedEvent mre(event.button.button);
			EventCallback(mre);
		}
		else if (event.type == SDL_MOUSEMOTION)
		{
			xRel = static_cast<float>(event.motion.xrel);
			yRel = -static_cast<float>(event.motion.yrel);
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

		SDL_memcpy(mPrevTriggerState, mCurrentTriggerState, 6 * sizeof(bool));
		SDL_memcpy(mPrevControllerState, mCurrentControllerState, 15 * sizeof(bool));

		xRel = 0.0f; yRel = 0.0f;

		while (SDL_PollEvent(&e) != 0)
		{
			ImGui_ImplSDL2_ProcessEvent(&e);

			HandleWindowEvents(e);
			HandleKeyboardEvents(e);
			HandleMouseEvents(e);
			HandleControllerEvents(e);
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