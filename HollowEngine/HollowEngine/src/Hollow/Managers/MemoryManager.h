#pragma once
#include "Hollow/Common.h"

namespace Hollow
{
	class Component;
	class SceneManager;
	
	/// <summary>
	/// Class Memory Manager.
	/// </summary>
	class MemoryManager
	{
		SINGLETON(MemoryManager)
	public:
		/// <summary>
		/// Initializes manager data.
		/// </summary>
		HOLLOW_API void Init(rapidjson::Value::Object& data);
		
		/// <summary>
		/// Registers the component.
		/// </summary>
		/// <param name="name">The component name.</param>
		/// <param name="component">The component.</param>
		HOLLOW_API void RegisterComponent(std::string name, Component* component);
		
		/// <summary>
		/// Cleans manager data.
		/// </summary>
		HOLLOW_API void CleanUp();
	
		/// <summary>
		/// Returns empty component from pool.
		/// </summary>
		/// <param name="name">The name.</param>
		/// <returns>Hollow.Component *.</returns>
		HOLLOW_API Component* NewComponent(std::string name);
		
		/// <summary>
		/// Clears the component and returns it to the pool.
		/// </summary>
		/// <param name="component">The component.</param>
		HOLLOW_API void DeleteComponent(Component* component);

		/// <summary>
		/// Returns empty game object from pool.
		/// </summary>
		/// <returns>Hollow.GameObject *.</returns>
		HOLLOW_API GameObject* NewGameObject();
		
		/// <summary>
		///Clears the game object and returns it to the pool.
		/// </summary>
		/// <param name="gameobject">The gameobject.</param>
		HOLLOW_API void DeleteGameObject(GameObject* gameobject);

		friend class SceneManager;
	private:
		std::unordered_map<std::string,std::list<Component*>> mComponentPool;
		std::list<GameObject*> mGameObjectPool;
		std::vector<std::pair<std::string, Component*>> mComponents;
	};
}
