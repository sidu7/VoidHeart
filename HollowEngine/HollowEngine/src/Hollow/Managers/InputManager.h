#pragma once
#include "Hollow/Events/Event.h"

namespace Hollow {

	class HOLLOW_API InputManager
	{
		SINGLETON(InputManager)		
	public:
		bool IsKeyPressed(unsigned int keycode);
		bool IsKeyReleased(unsigned int keycode);
		bool IsKeyTriggered(unsigned int keycode);
		bool IsMouseButtonPressed(unsigned int button);
		glm::ivec2 GetMousePosition();
		float GetMouseX();
		float GetMouseY();

		using EventCallbackFn = std::function<void(Event&)>;

		void SetEventCallback(const EventCallbackFn& callback);
		
		void Init();
		void Update();
	
		
	private:
		EventCallbackFn EventCallback;
		
		void HandleKeyboardEvents(const SDL_Event& e);
		void HandleMouseEvents(const SDL_Event& e);
		void HandleWindowEvents(const SDL_Event& e);

		
		Uint8 mCurrentState[512];
		Uint8 mPreviousState[512];
		bool mPrevMouseState[3];
		bool mCurrentMouseState[3];
	};

}
