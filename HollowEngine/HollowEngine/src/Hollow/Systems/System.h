#pragma once
#include <vector>
#include "Hollow/Managers/SystemManager.h"
#include "Hollow/Core/GameObject.h"


namespace Hollow
{
	class GameEvent;
	
	class HOLLOW_API System
	{
	public:
		System(System* instance,int tier) {
			SystemManager::Instance().RegisterSystem(instance);
			this->mTier = tier;
		}
		virtual ~System() {}
		virtual void Init() {}
		virtual void Update() = 0;
		virtual std::type_index GetIndex() = 0;
		virtual void AddGameObject(GameObject* pGameObject) = 0;
		virtual void HandleBroadcastEvent(GameEvent* event) { }
		virtual void OnDeleteGameObject(GameObject* pGameObject) {}

		void DeleteGameObject(GameObject* pGameObject)
		{
			mGameObjects.erase(std::find(mGameObjects.begin(), mGameObjects.end(), pGameObject));
			OnDeleteGameObject(pGameObject);
		}

		void DeleteAllGameObjects() 
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