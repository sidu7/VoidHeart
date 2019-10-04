#include <hollowpch.h>

#include "SystemManager.h"
#include "Hollow/Systems/System.h"

namespace Hollow
{
	SystemManager::~SystemManager()
	{
		mSystems.clear();
	}

	void SystemManager::Init()
	{
		std::sort(mSystems.begin(), mSystems.end(), [](System* x, System* y) { return x->mTier < y->mTier; });
	}

	void SystemManager::Update()
	{
		for (unsigned int i = 0; i < mSystems.size(); ++i)
		{
			mSystems[i]->Update();
		}
	}
	void SystemManager::AddObjectToSystems(GameObject* GameObject)
	{
		for (unsigned int i = 0; i < mSystems.size(); ++i)
		{
			mSystems[i]->AddGameObject(GameObject);
		}
	}
	void SystemManager::DeleteGameObejectInSystems(GameObject* GameObject)
	{
		for (unsigned int i = 0; i < mSystems.size(); ++i)
		{
			mSystems[i]->DeleteGameObject(GameObject);
		}
	}
	void SystemManager::DeleteAllGameObjectsInSystems()
	{
		for (unsigned int i = 0; i < mSystems.size(); ++i)
		{
			mSystems[i]->DeleteAllGameObjects();
		}
	}
}
