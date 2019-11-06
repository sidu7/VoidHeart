#pragma once
#include "Hollow/Systems/System.h"

namespace Hollow
{
	class Component;

	class HOLLOW_API MemoryManager
	{
		SINGLETON(MemoryManager)
	public:
		void Init(rapidjson::Value::Object& data);
		void RegisterComponent(std::string name, Component* component);
		void CleanUp();
	
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