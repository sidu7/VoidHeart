#pragma once
#include <vector>


namespace Hollow
{
	class GameObject;
	class System;
	class GameEvent;
	class SystemManager
	{
		SINGLETON(SystemManager)
	public:
		~SystemManager();

		void RegisterSystem(System* system, std::type_index index);

		void Init();
		void Update();
		void CleanUp();
		void AddObjectToSystems(GameObject* GameObject);
		void BroadcastEventToSystems(GameEvent* event);
		void DeleteGameObejectInSystems(GameObject* GameObject);
		void DeleteAllGameObjectsInSystems();

		template<typename T>
		System* GetSystem()
		{
			if (mSystemMap.find(std::type_index(typeid(T))) != mSystemMap.end())
			{
				return static_cast<T*>(mSystemMap[std::type_index(typeid(T))]);
			}
			else
			{
				return nullptr;
			}
		}

	private:
		std::vector<System*> mSystems;
		std::unordered_map<std::type_index, System*> mSystemMap;
	};
}