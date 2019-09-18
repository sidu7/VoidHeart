#include "hollowpch.h"
#include "GameObjectManager.h"

void Hollow::GameObjectManager::AddGameObject(GameObject* object)
{
	mGameObjects.push_back(object);
	
}

void Hollow::GameObjectManager::DeleteGameObject(GameObject* object)
{
}

void Hollow::GameObjectManager::DeleteAllGameObjects()
{
}
