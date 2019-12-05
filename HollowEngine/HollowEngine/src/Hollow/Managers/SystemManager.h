#pragma once
#include "Hollow/Common.h"

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
		
		HOLLOW_API  void RegisterSystem(System* system, std::type_index index);
		  
		HOLLOW_API  void Init();
		HOLLOW_API  void OnSceneInit();
		HOLLOW_API  void Update();
		HOLLOW_API  void CleanUp();
		HOLLOW_API  void AddObjectToSystems(GameObject* GameObject);
		HOLLOW_API  void BroadcastEventToSystems(GameEvent& event);
		HOLLOW_API  void DeleteGameObejectInSystems(GameObject* GameObject);
		HOLLOW_API  void DeleteAllGameObjectsInSystems();

		template<typename T>
		T* GetSystem()
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