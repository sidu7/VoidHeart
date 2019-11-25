#pragma once
#include "Hollow/Common.h"

namespace Hollow
{
	class Component;
	class SceneManager;
	
	class MemoryManager
	{
		SINGLETON(MemoryManager)
	public:
		HOLLOW_API void Init(rapidjson::Value::Object& data);
		HOLLOW_API void RegisterComponent(std::string name, Component* component);
		HOLLOW_API void CleanUp();
	
		HOLLOW_API Component* NewComponent(std::string name);
		HOLLOW_API void DeleteComponent(Component* component);

		HOLLOW_API GameObject* NewGameObject();
		HOLLOW_API void DeleteGameObject(GameObject* gameobject);

		friend class SceneManager;
	private:
		std::unordered_map<std::string,std::list<Component*>> mComponentPool;
		std::list<GameObject*> mGameObjectPool;
		std::vector<std::pair<std::string, Component*>> mComponents;
	};
}
