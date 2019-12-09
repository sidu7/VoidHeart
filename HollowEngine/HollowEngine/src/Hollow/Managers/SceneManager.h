#pragma once
#include "Hollow/Common.h"

namespace Hollow
{
	class Component;
	
	/// <summary>
	/// Class Scene Manager.
	/// </summary>
	class SceneManager
	{
		SINGLETON(SceneManager);
	public:
		/// <summary>
		/// Initializes manager data.
		/// </summary>
		HOLLOW_API void Init();
		
		/// <summary>
		/// Cleans manager data.
		/// </summary>
		HOLLOW_API void CleanUp();
		
		/// <summary>
		/// Displays debug window.
		/// </summary>
		HOLLOW_API void DebugDisplay();

		/// <summary>
		/// Loads the level.
		/// </summary>
		/// <param name="LevelFile">The level name.</param>
		HOLLOW_API void LoadLevel(std::string LevelFile);

	private:
		void DeserializeGameObject();
		void DeserializeLevel();
		
	private:
		// Creating Prefab
		std::string mSelectedComponentName;
		char charBuffer[255];
		std::string mBackgroundAudio;
		GameObject* mSelectedGameObject;
		Component* mSelectedComponent;
	};
}
