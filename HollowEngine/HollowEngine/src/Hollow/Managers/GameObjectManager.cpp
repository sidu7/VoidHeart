#include "hollowpch.h"
#include "GameObjectManager.h"
#include "SystemManager.h"
#include "..//GameObject.h"

namespace Hollow {
	void GameObjectManager::AddGameObject(GameObject* GameObject)
	{
		mGameObjects.push_back(GameObject);
		SystemManager::Instance().AddObjectToSystems(GameObject);
	}

	void GameObjectManager::DeleteGameObject(GameObject* GameObject)
	{
		mGameObjects.erase(std::find(mGameObjects.begin(), mGameObjects.end(), GameObject));
		SystemManager::Instance().DeleteGameObejectInSystems(GameObject);
	}

	void GameObjectManager::DeleteAllGameObjects()
	{
		mGameObjects.clear();
		SystemManager::Instance().DeleteAllGameObjectsInSystems();
	}

}
