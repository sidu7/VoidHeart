#pragma once
#include <Hollow\Systems\System.h>

#define MAX_OBJECTS 1000

namespace Hollow
{
	class Component;

	class MemoryManager
	{
		SINGLETON(MemoryManager)
	public:
		void Init();
		void RegisterComponent(const char* name, Component* component)
		{
			mComponentPool[name].push_back(component);
		}

		Component* NewComponent(const char* name);
		void DeleteComponent(Component* component);

		GameObject* NewGameObject();
		void DeleteGameObject(GameObject* gameobject);

	public:
		std::unordered_map<const char*,std::list<Component*>> mComponentPool;
		std::list<GameObject*> mGameObjectPool;
	};
}