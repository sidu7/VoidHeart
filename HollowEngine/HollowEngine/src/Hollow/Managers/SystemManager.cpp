#include <hollowpch.h>

#include "SystemManager.h"
#include "Hollow/Systems/System.h"
#include "Hollow/Events/GameEvent.h"

namespace Hollow
{
	SystemManager::~SystemManager()
	{
		mSystems.clear();
	}

	void SystemManager::RegisterSystem(System* system,std::type_index index)
	{
		mSystems.push_back(system);
		mSystemMap[index] = system;
	}

	void SystemManager::Init()
	{
		std::sort(mSystems.begin(), mSystems.end(), [](System* x, System* y) { return x->mTier < y->mTier; });
		std::for_each(mSystems.begin(), mSystems.end(), [](System* x) { x->Init(); });
	}

	void SystemManager::OnSceneInit()
	{
		for (unsigned int i = 0; i < mSystems.size(); ++i)
		{
			mSystems[i]->OnSceneInit();
		}
	}

	void SystemManager::Update()
	{
		for (unsigned int i = 0; i < mSystems.size(); ++i)
		{
			mSystems[i]->Update();
		}
	}

	void SystemManager::CleanUp()
	{
		mSystems.clear();
		mSystemMap.clear();
	}

	void SystemManager::AddObjectToSystems(GameObject* GameObject)
	{
		for (unsigned int i = 0; i < mSystems.size(); ++i)
		{
			mSystems[i]->AddGameObject(GameObject);
		}
	}

	void SystemManager::BroadcastEventToSystems(GameEvent& event)
	{
		for (unsigned int i = 0; i < mSystems.size(); ++i)
		{
			mSystems[i]->HandleBroadcastEvent(event);
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
