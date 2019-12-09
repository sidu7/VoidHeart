#pragma once
#include <vector>
#include "Hollow/Common.h"
#include "Hollow/Managers/SystemManager.h"
#include "Hollow/Core/GameObject.h"


namespace Hollow
{
	class GameEvent;
	
	/// <summary>
	/// Class System.
	/// </summary>
	class System
	{
	public:
		/// <summary>
		/// Initializes a new instance of the <see cref="System"/> class.
		/// </summary>
		/// <param name="instance">The instance of System type.</param>
		/// <param name="tier">The tier for update call.</param>
		/// <param name="index">The unique type identifier.</param>
		HOLLOW_API System(System* instance, int tier, std::type_index index) {
			SystemManager::Instance().RegisterSystem(instance,index);
			this->mTier = tier;
		}
		/// <summary>
		/// Destructor for <see cref="System"/> class.
		/// </summary>
		HOLLOW_API virtual ~System() {}
		/// <summary>
		/// Initializes the System.
		/// </summary>
		HOLLOW_API virtual void Init() {}
		/// <summary>
		/// Called when a scene is initialized.
		/// </summary>
		HOLLOW_API virtual void OnSceneInit() {}
		/// <summary>
		/// Updates the System every frame.
		/// </summary>
		HOLLOW_API virtual void Update() = 0;
		/// <summary>
		/// Adds the game object.
		/// </summary>
		/// <param name="pGameObject">The gameObject instance to add.</param>
		HOLLOW_API virtual void AddGameObject(GameObject* pGameObject) = 0;
		/// <summary>
		/// Handles the broadcast event.
		/// </summary>
		/// <param name="event">The event.</param>
		HOLLOW_API virtual void HandleBroadcastEvent(GameEvent& event) { }
		/// <summary>
		/// Called when a game object is deleted.
		/// </summary>
		/// <param name="pGameObject">The gameObject instance to delete.</param>
		HOLLOW_API virtual void OnDeleteGameObject(GameObject* pGameObject) {}
		/// <summary>
		/// Called when all game objects have to be deleted.
		/// </summary>
		HOLLOW_API virtual void OnDeleteAllGameObjects() {}

		/// <summary>
		/// Deletes the game object.
		/// </summary>
		/// <param name="pGameObject">The game object instance to delete.</param>
		HOLLOW_API void DeleteGameObject(GameObject* pGameObject)
		{
			auto itr = std::find(mGameObjects.begin(), mGameObjects.end(), pGameObject);
			if (itr != mGameObjects.end())
			{
				mGameObjects.erase(itr);
				OnDeleteGameObject(pGameObject);
			}
		}

		/// <summary>
		/// Deletes all game objects.
		/// </summary>
		HOLLOW_API void DeleteAllGameObjects()
		{
			mGameObjects.clear();
			OnDeleteAllGameObjects();
		}
	protected:
		template<typename First> // 1 template parameter
		bool CheckAllComponents(GameObject* pGameObject)
		{
			if (pGameObject->GetComponent<First>())
			{
				mGameObjects.push_back(pGameObject);
				return true;
			}

			return false;
		}

		template<typename First, typename Second, typename ... Rest> // >=2 template parameters
		bool CheckAllComponents(GameObject* pGameObject)
		{
			if (pGameObject->GetComponent<First>())
			{
				return CheckAllComponents<Second, Rest...>(pGameObject);
			}

			return false;
		}

		template<typename First> // 1 template parameter
		bool CheckAnyComponents(GameObject* pGameObject)
		{
			if (pGameObject->GetComponent<First>())
			{
				mGameObjects.push_back(pGameObject);
				return true;
			}
			return false;
		}

		template<typename First, typename Second, typename ... Rest> // >=2 template parameters
		bool CheckAnyComponents(GameObject* pGameObject)
		{
			if (pGameObject->GetComponent<First>())
			{
				mGameObjects.push_back(pGameObject);
				return true;
			}
			return CheckAnyComponents<Second, Rest...>(pGameObject);
		}

	public:
		int mTier;

	protected:
		std::vector<GameObject*> mGameObjects;
	};
}