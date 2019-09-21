#pragma once
#include "Hollow/Systems/System.h"

#define MAX_OBJECTS 1000

namespace Hollow
{
	class Component;

	class HOLLOW_API MemoryManager
	{
		SINGLETON(MemoryManager)
	public:
		void Init();
		void RegisterComponent(std::string name, Component* component);
	
		Component* NewComponent(std::string name);
		void DeleteComponent(Component* component);

		GameObject* NewGameObject();
		void DeleteGameObject(GameObject* gameobject);

	private:
		std::unordered_map<std::string,std::list<Component*>> mComponentPool;
		std::list<GameObject*> mGameObjectPool;
		std::vector<std::pair<std::string, Component*>> mComponents;
	};
}