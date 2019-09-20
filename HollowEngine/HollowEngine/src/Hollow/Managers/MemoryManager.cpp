#include <hollowpch.h>
#include "MemoryManager.h"
#include "../Components/Component.h"
#include "../GameObject.h"

namespace Hollow
{
	Component* MemoryManager::NewComponent(const char* name)
	{
		Component* comp = mComponentPool[name].front();
		mComponentPool[name].pop_front();
		return comp;
	}

	void MemoryManager::DeleteComponent(Component* component)
	{
		mComponentPool[component->mComponentName.c_str()].push_back(component);
	}

	GameObject* MemoryManager::NewGameObject()
	{
		GameObject* object = mGameObjectPool.front();
		object->mActive = true;
		mGameObjectPool.pop_front();
		return object;
	}

	void MemoryManager::DeleteGameObject(GameObject* gameobject)
	{
		gameobject->mActive = false;
		mGameObjectPool.push_back(gameobject);
	}

	void MemoryManager::Init()
	{
		for (auto list : mComponentPool)
		{
			Component* component = list.second.front();
			for (unsigned int i = 0; i < MAX_OBJECTS - 1; ++i)
			{
				list.second.push_back(component->CreateComponent());
			}
		}

		for (unsigned int i = 0; i < MAX_OBJECTS; ++i)
		{
			mGameObjectPool.push_back(new GameObject());
		}
	}
}