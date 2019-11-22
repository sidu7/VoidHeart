#pragma once
#include <vector>
#include "Hollow/Common.h"
#include "Hollow/Managers/SystemManager.h"
#include "Hollow/Core/GameObject.h"


namespace Hollow
{
	class GameEvent;
	
	class System
	{
	public:
		HOLLOW_API System(System* instance,int tier, std::type_index index) {
			SystemManager::Instance().RegisterSystem(instance,index);
			this->mTier = tier;
		}
		HOLLOW_API virtual ~System() {}
		HOLLOW_API virtual void Init() {}
		HOLLOW_API virtual void Update() = 0;
		HOLLOW_API virtual void AddGameObject(GameObject* pGameObject) = 0;
		HOLLOW_API virtual void HandleBroadcastEvent(GameEvent* event) { }
		HOLLOW_API virtual void OnDeleteGameObject(GameObject* pGameObject) {}

		HOLLOW_API void DeleteGameObject(GameObject* pGameObject)
		{
			mGameObjects.erase(std::find(mGameObjects.begin(), mGameObjects.end(), pGameObject));
			OnDeleteGameObject(pGameObject);
		}

		HOLLOW_API void DeleteAllGameObjects()
		{
			mGameObjects.clear();
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