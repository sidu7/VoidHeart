#pragma once
#include "Hollow/Common.h"

namespace Hollow{
	class GameObject;
	class GameObjectManager 
	{
		SINGLETON(GameObjectManager);
	public:
		HOLLOW_API void AddGameObject(GameObject* GameObject);
		HOLLOW_API std::vector<GameObject*> GetObjectByType(int ObjectType);
		HOLLOW_API void DeleteGameObject(GameObject* GameObject);
		HOLLOW_API void DeleteAllGameObjects();
		HOLLOW_API const std::vector<GameObject*>& GetGameObjects() { return mGameObjects; }

		HOLLOW_API void ClearDeletionList();
		
	private:
		std::vector<GameObject*> mGameObjects;

		std::vector<GameObject*> mDeletionList;
	};
}