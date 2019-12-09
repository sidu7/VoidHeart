#pragma once
#include "Hollow/Common.h"

namespace Hollow
{
	class GameObject;
	class System;
	class GameEvent;
	
	/// <summary>
	/// Class System Manager.
	/// </summary>
	class SystemManager
	{
		SINGLETON(SystemManager)
	public:
		
		/// <summary>
		/// Initializes manager data.
		/// </summary>
		HOLLOW_API  void Init();
		  
		/// <summary>
		/// Registers the system.
		/// </summary>
		/// <param name="system">The system.</param>
		/// <param name="index">The priority.</param>
		HOLLOW_API  void RegisterSystem(System* system, std::type_index index);
		
		/// <summary>
		/// Callback function when scene is loaded.
		/// </summary>
		HOLLOW_API  void OnSceneInit();
		
		/// <summary>
		/// Updates all systems.
		/// </summary>
		HOLLOW_API  void Update();
		
		/// <summary>
		/// Cleans manager data.
		/// </summary>
		HOLLOW_API  void CleanUp();
		
		/// <summary>
		/// Adds the object to systems.
		/// </summary>
		/// <param name="GameObject">The game object.</param>
		HOLLOW_API  void AddObjectToSystems(GameObject* GameObject);
		
		/// <summary>
		/// Broadcasts the event to systems.
		/// </summary>
		/// <param name="event">The event.</param>
		HOLLOW_API  void BroadcastEventToSystems(GameEvent& event);
		
		/// <summary>
		/// Deletes the game object in systems.
		/// </summary>
		/// <param name="GameObject">The game object.</param>
		HOLLOW_API  void DeleteGameObjectInSystems(GameObject* GameObject);
		
		/// <summary>
		/// Deletes all game objects in systems.
		/// </summary>
		HOLLOW_API  void DeleteAllGameObjectsInSystems();

		/// <summary>
		/// Gets the [template parameter] system.
		/// </summary>
		/// <returns>System *.</returns>
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