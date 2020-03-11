#include "hollowpch.h"

#include "GameObjectManager.h"
#include "SystemManager.h"
#include "MemoryManager.h"

#include "Hollow/Core/GameObject.h"

namespace Hollow {
	void GameObjectManager::AddGameObject(GameObject* GameObject)
	{
#if HW_DEBUG
		auto itr = std::find(mGameObjects.begin(), mGameObjects.end(), GameObject);
		if (itr == mGameObjects.end())
		{
#endif
			mGameObjects.push_back(GameObject);
#if HW_DEBUG
		}
#endif
		SystemManager::Instance().AddObjectToSystems(GameObject);
	}

	std::vector<GameObject*> GameObjectManager::GetObjectByType(int ObjectType)
	{
		std::vector<GameObject*> list;
		for (int i = 0; i < mGameObjects.size(); ++i)
		{
			if (mGameObjects[i]->mType == ObjectType)
			{
				list.emplace_back(mGameObjects[i]);
			}
		}
		return list;
	}

	void GameObjectManager::DeleteGameObject(GameObject* GameObject)
	{
		if (std::find(mDeletionList.begin(), mDeletionList.end(), GameObject) != mDeletionList.end())
			return;

		mDeletionList.push_back(GameObject);
	}

	void GameObjectManager::ClearDeletionList()
	{
		if (!mDeletionList.empty())
		{
			for (auto GameObject : mDeletionList) {
				
				// Delete Everywhere
				mGameObjects.erase(std::find(mGameObjects.begin(), mGameObjects.end(), GameObject));
				SystemManager::Instance().DeleteGameObjectInSystems(GameObject);

				// Destroy
				GameObject->Destroy();

				// Reclaim
				MemoryManager::Instance().DeleteGameObject(GameObject);
			}

			mDeletionList.clear();
		}
	}
	
	void GameObjectManager::DeleteAllGameObjects()
	{
		mDeletionList.clear();
		for (unsigned int i = 0; i < mGameObjects.size(); ++i)
		{
			mGameObjects[i]->Destroy();
			MemoryManager::Instance().DeleteGameObject(mGameObjects[i]);
		}
		mGameObjects.clear();
		SystemManager::Instance().DeleteAllGameObjectsInSystems();
	}

}
