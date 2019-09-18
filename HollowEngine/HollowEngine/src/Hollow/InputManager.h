#pragma once

#include "Core.h"
#include <utility>

namespace Hollow {

	class HOLLOW_API InputManager : public Singleton<InputManager>
	{
		friend class Singleton<InputManager>;
		
	public:
		bool IsKeyPressed(unsigned int keycode);
		bool IsKeyTriggered(unsigned int keycode);
		bool IsMouseButtonPressed(unsigned int button);
		std::pair<float, float> GetMousePosition();
		float GetMouseX();
		float GetMouseY();

		void Init();
		void Update();
	
		
	private:
		InputManager() {}
		Uint8 mCurrentState[512];
		Uint8 mPreviousState[512];
		bool mPrevMouseState[3];
		bool mCurrentMouseState[3];
	};

}
