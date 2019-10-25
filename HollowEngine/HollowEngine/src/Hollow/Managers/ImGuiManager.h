#pragma once

namespace Hollow {
	class GameWindow;
	class GameObject;

	class HOLLOW_API ImGuiManager{
		SINGLETON(ImGuiManager)
	public:
		void Init(GameWindow* pWindow);
		void CleanUp();
		void Update();
		// TODO: Write cleanup function
		void StartFrame();
	private:
		void Render();
	private:
		GameWindow* mpWindow;
		GameObject* mpSelectedGameObject;
		unsigned int mSelectedGameObjectID;
	};
}