#include <hollowpch.h>
#include "MemoryManager.h"
#include "Hollow/Components/Component.h"
#include "Hollow/Core/GameObject.h"

namespace Hollow
{
	void MemoryManager::RegisterComponent(std::string name, Component* component)
	{
		mComponents.push_back(std::make_pair(name, component));
	}

	Component* MemoryManager::NewComponent(std::string name)
	{
		Component* comp = nullptr;
		if (mComponentPool.find(name) != mComponentPool.end())
		{
			comp = mComponentPool[name].front();
			mComponentPool[name].pop_front();
			comp->Init();
		}
		else
		{
			HW_CORE_ERROR("Component {0} not found", name);
		}
		return comp;
	}

	void MemoryManager::DeleteComponent(Component* component)
	{
		component->Clear();
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

	void MemoryManager::Init(rapidjson::Value::Object& data)
	{
		// Create Components Pool
		unsigned int maxComponents = data["ComponentsPool"].GetUint();
		for (unsigned int i = 0; i < mComponents.size(); ++i)
		{
			Component* component = mComponents[i].second->CreateComponent();
			std::list<Component*> newlist;
			for (unsigned int i = 0; i < maxComponents; ++i)
			{
				newlist.push_back(component->CreateComponent());
			}
			mComponentPool[component->mComponentName.c_str()] = newlist;
		}

		//Create GameObjects Pool
		unsigned int maxObjects = data["GameobjectsPool"].GetUint();
		for (unsigned int i = 0; i < maxObjects; ++i)
		{
			mGameObjectPool.push_back(new GameObject());
		}
	}
}