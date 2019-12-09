#pragma once
#include "Hollow/Common.h"

namespace Hollow {
	class GameWindow;
	class GameObject;

	/// <summary>
	/// Class Debug GUI Manager.
	/// </summary>
	class ImGuiManager {
		SINGLETON(ImGuiManager)
	public:
		/// <summary>
		/// Initializes manager data.
		/// </summary>
		HOLLOW_API void Init(GameWindow* pWindow);
		
		/// <summary>
		/// Cleans manager data.
		/// </summary>
		HOLLOW_API void CleanUp();
		
		/// <summary>
		/// Renders Debug GUI.
		/// </summary>
		HOLLOW_API void Update();
				
		/// <summary>
		/// Starts the Debug GUI frame.
		/// </summary>
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