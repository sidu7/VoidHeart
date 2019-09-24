#pragma once
#include "hollowpch.h"

namespace Hollow{
	class GameObject;
	class HOLLOW_API GameObjectManager 
	{
		SINGLETON(GameObjectManager);
	public:
		void AddGameObject(GameObject* GameObject);
		void DeleteGameObject(GameObject* GameObject);
		void DeleteAllGameObjects();
		const std::vector<GameObject*>& GetGameObjects() { return mGameObjects; }

	private:
		std::vector<GameObject*> mGameObjects;
	};
}