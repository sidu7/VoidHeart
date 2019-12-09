#pragma once
#include "Hollow/Core/Core.h"
#include "Hollow/Events/Event.h"
#include "Hollow/Common.h"

namespace Hollow {

	/// <summary>
	/// Class Input Manager.
	/// </summary>
	class InputManager
	{
		SINGLETON(InputManager)		
	public:
		/// <summary>
		/// Determines whether [the specified key] is pressed.
		/// </summary>
		/// <param name="keycode">The keycode.</param>
		/// <returns>bool.</returns>
		HOLLOW_API bool IsKeyPressed(unsigned int keycode);
		
		/// <summary>
		/// Determines whether [the specified key] is released.
		/// </summary>
		/// <param name="keycode">The keycode.</param>
		/// <returns>bool.</returns>
		HOLLOW_API bool IsKeyReleased(unsigned int keycode);
		
		/// <summary>
		/// Determines whether [the specified key] is triggered.
		/// </summary>
		/// <param name="keycode">The keycode.</param>
		/// <returns>bool.</returns>
		HOLLOW_API bool IsKeyTriggered(unsigned int keycode);
		
		/// <summary>
		/// Determines whether [the specified mouse button] is pressed.
		/// </summary>
		/// <param name="button">The button code.</param>
		/// <returns>bool.</returns>
		HOLLOW_API bool IsMouseButtonPressed(unsigned int button);
		
		/// <summary>
		/// Determines whether [the specified mouse button] is triggered.
		/// </summary>
		/// <param name="button">The button code.</param>
		/// <returns>bool.</returns>
		HOLLOW_API bool IsMouseButtonTriggered(unsigned int button);
		
		/// <summary>
		/// Determines whether [the specified controller button] is pressed.
		/// </summary>
		/// <param name="button">The button code.</param>
		/// <returns>bool.</returns>
		HOLLOW_API bool IsControllerButtonPressed(SDL_GameControllerButton button);
		
		/// <summary>
		/// Determines whether [the specified controller button] is triggered.
		/// </summary>
		/// <param name="button">The button code.</param>
		/// <returns>bool.</returns>
		HOLLOW_API bool IsControllerButtonTriggered(SDL_GameControllerButton button);
		
		/// <summary>
		/// Determines whether [the specified controller button] is released.
		/// </summary>
		/// <param name="button">The button code.</param>
		/// <returns>bool.</returns>
		HOLLOW_API bool IsControllerButtonReleased(SDL_GameControllerButton button);
		
		/// <summary>
		/// Determines whether [the specified controller trigger] is pressed.
		/// </summary>
		/// <param name="button">The controller axis.</param>
		/// <returns>bool.</returns>
		HOLLOW_API bool IsControllerTriggerPressed(SDL_GameControllerAxis axis);
		
		/// <summary>
		/// Determines whether [the specified controller trigger] is triggered.
		/// </summary>
		/// <param name="axis">The controller axis.</param>
		/// <returns>bool.</returns>
		HOLLOW_API bool IsControllerTriggerTriggered(SDL_GameControllerAxis axis);
		
		/// <summary>
		/// Determines whether [the specified controller trigger] is released.
		/// </summary>
		/// <param name="axis">The controller axis.</param>
		/// <returns>bool.</returns>
		HOLLOW_API bool IsControllerTriggerReleased(SDL_GameControllerAxis axis);
		
		/// <summary>
		/// Gets the axis value.
		/// </summary>
		/// <param name="axis">The axis.</param>
		/// <returns>short int.</returns>
		HOLLOW_API Sint16 GetAxisValue(SDL_GameControllerAxis axis);
		
		/// <summary>
		/// Gets the mouse velocity.
		/// </summary>
		/// <returns>glm.vec2.</returns>
		HOLLOW_API glm::vec2 GetMouseMotion();
		
		/// <summary>
		/// Gets the mouse position.
		/// </summary>
		/// <returns>glm.ivec2.</returns>
		HOLLOW_API glm::ivec2 GetMousePosition();
    
		/// <summary>
		/// Gets the mouse x position.
		/// </summary>
		/// <returns>float.</returns>
		HOLLOW_API float GetMouseX();
		
		/// <summary>
		/// Gets the mouse y position.
		/// </summary>
		/// <returns>float.</returns>
		HOLLOW_API float GetMouseY();

		using EventCallbackFn = std::function<void(Event&)>;

		/// <summary>
		/// Sets the event callback.
		/// </summary>
		/// <param name="callback">The callback function.</param>
		HOLLOW_API void SetEventCallback(const EventCallbackFn& callback);
		
		/// <summary>
		/// Initializes manager data.
		/// </summary>
		HOLLOW_API void Init();
		
		/// <summary>
		/// Updates manager data.
		/// </summary>
		HOLLOW_API void Update();
		
		/// <summary>
		/// Hides the mouse cursor.
		/// </summary>
		HOLLOW_API void HideMouseCursor();
		
		/// <summary>
		/// Shows the mouse cursor.
		/// </summary>
		HOLLOW_API void ShowMouseCursor();
		
		SDL_GameController* mpController;
	private:
		EventCallbackFn EventCallback;
		
		void HandleKeyboardEvents(const SDL_Event& e);
		void HandleMouseEvents(const SDL_Event& e);
		void HandleWindowEvents(const SDL_Event& e);
		void HandleControllerEvents(const SDL_Event& e);

		float xRel, yRel;
		
		Uint8 mCurrentState[512];
		Uint8 mPreviousState[512];

		bool mCurrentControllerState[15];
		bool mPrevControllerState[15];
		bool mPrevMouseState[6];
		bool mCurrentMouseState[6];

		bool mCurrentTriggerState[6];
		bool mPrevTriggerState[6];
	};

}
