#pragma once
#include "Hollow/Common.h"

namespace Hollow {
	class GameWindow;
	class GameObject;

	class ImGuiManager{
		SINGLETON(ImGuiManager)
	public:
		HOLLOW_API void Init(GameWindow* pWindow);
		HOLLOW_API void CleanUp();
		HOLLOW_API void Update();
		// TODO: Write cleanup function
		HOLLOW_API void StartFrame();
		
		GameObject* mpSelectedGameObject;
		unsigned int mSelectedGameObjectID;
	private:
		void Render();
		void TestingDisplay();
	private:
		GameWindow* mpWindow;
	};
}