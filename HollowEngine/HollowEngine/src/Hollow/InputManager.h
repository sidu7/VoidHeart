#pragma once

#include "Core.h"
#include <utility>

namespace Hollow {

	class HOLLOW_API InputManager : public Singleton<InputManager>
	{
		friend class Singleton<InputManager>;
		
	public:
		bool IsKeyPressed(int keycode);
		bool IsKeyTriggered(int keycode);
		bool IsMouseButtonPressed(int button);
		std::pair<float, float> GetMousePosition();
		float GetMouseX();
		float GetMouseY();

		void Init();
		void Update();
	
		
	private:
		InputManager() {}
		unsigned int mCurrentState[512];
		unsigned int mPreviousState[512];
		bool mPrevMouseState[3];
		bool mCurrentMouseState[3];
	};

}
