#pragma once
#include "Hollow/Common.h"

namespace Hollow{
	class GameObject;
	
	/// <summary>
	/// Class GameObject Manager.
	/// </summary>
	class GameObjectManager
	{
		SINGLETON(GameObjectManager);
	public:
		/// <summary>
		/// Adds the game object.
		/// </summary>
		/// <param name="GameObject">The game object.</param>
		HOLLOW_API void AddGameObject(GameObject* GameObject);
		
		/// <summary>
		/// Gets list of game objects by the type.
		/// </summary>
		/// <param name="ObjectType">Type of the game object.</param>
		/// <returns>list of game objects;.</returns>
		HOLLOW_API std::vector<GameObject*> GetObjectByType(int ObjectType);
		
		/// <summary>
		/// Deletes the game object.
		/// </summary>
		/// <param name="GameObject">The game object.</param>
		HOLLOW_API void DeleteGameObject(GameObject* GameObject);
		
		/// <summary>
		/// Deletes all game objects.
		/// </summary>
		HOLLOW_API void DeleteAllGameObjects();

		/// <summary>
		/// Deletes all games objects except those in a vector
		/// </summary>
		HOLLOW_API void DeleteAllGameObjectsExcept(const std::vector<GameObject*>& gameObjectExceptions);
		
		/// <summary>
		/// Gets the game objects.
		/// </summary>
		/// <returns>const std.vector&lt;GameObject*&gt;&.</returns>
		HOLLOW_API const std::vector<GameObject*>& GetGameObjects() { return mGameObjects; }
		
		/// <summary>
		/// Clears the deletion list.
		/// </summary>
		HOLLOW_API void ClearDeletionList();
		
	private:
		std::vector<GameObject*> mGameObjects;

		std::vector<GameObject*> mDeletionList;
	};
}