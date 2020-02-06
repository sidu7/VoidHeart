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

	void MemoryManager::CleanUp()
	{
		for (unsigned int i = 0; i < mComponents.size(); ++i)
		{
			auto& list = mComponentPool[mComponents[i].first];
			for (unsigned int j = 0; j < list.size(); ++i)
			{
				Component* comp = list.front();
				comp->Clear();
				delete comp;
				list.pop_front();
			}
		}

		for (unsigned int i = 0; i < mGameObjectPool.size(); ++i)
		{
			delete mGameObjectPool.front();
			mGameObjectPool.pop_front();
		}
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
		component->mpOwner = nullptr;
		mComponentPool[component->mComponentName.c_str()].push_back(component);
	}

	GameObject* MemoryManager::NewGameObject()
	{
		if (mGameObjectPool.size() > 0)
		{
			GameObject* object = mGameObjectPool.front();
			object->mActive = true;
			mGameObjectPool.pop_front();
			return object;
		}
		else
		{
			HW_CORE_ERROR("All GameObjects used");
			return nullptr;
		}
	}

	void MemoryManager::DeleteGameObject(GameObject* gameobject)
	{
		gameobject->mActive = false;
		mGameObjectPool.push_back(gameobject);
	}

	void MemoryManager::Init(rapidjson::Value::Object& data)
	{
		std::unordered_map<std::string, int> customComponentPool;

		auto components = data["CustomPool"].GetArray();

		for (int i = 0; i < components.Size(); ++i)
		{
			auto comp = components[i].GetArray();
			customComponentPool[comp[0].GetString()] = comp[1].GetInt();
		}

		// Create Components Pool
		unsigned int maxComponents = data["ComponentsPool"].GetUint();
		for (unsigned int i = 0; i < mComponents.size(); ++i)
		{
			Component* component = mComponents[i].second->CreateComponent();
			std::list<Component*> newlist;
			unsigned int size = maxComponents;
			if (customComponentPool.find(component->mComponentName) != customComponentPool.end())
			{
				size = customComponentPool[component->mComponentName];
			}
			for (unsigned int i = 0; i < size; ++i)
			{
				newlist.push_back(component->CreateComponent());
			}
			mComponentPool[component->mComponentName] = newlist;
		}

		//Create GameObjects Pool
		unsigned int maxObjects = data["GameobjectsPool"].GetUint();
		for (unsigned int i = 0; i < maxObjects; ++i)
		{
			mGameObjectPool.push_back(new GameObject());
		}
	}
}