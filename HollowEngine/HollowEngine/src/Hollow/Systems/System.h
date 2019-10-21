#pragma once
#include <vector>
#include "Hollow/Managers/SystemManager.h"
#include "Hollow/Core/GameObject.h"


namespace Hollow
{
	class HOLLOW_API System
	{
	public:
		System(System* instance,int tier) {
			SystemManager::Instance().RegisterSystem(instance);
			this->mTier = tier;
		}
		virtual ~System() {}
		virtual void Update() = 0;
		virtual void AddGameObject(GameObject* pGameObject) = 0;
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
		bool CheckComponents(GameObject* pGameObject)
		{
			if (pGameObject->GetComponent<First>())
			{
				mGameObjects.push_back(pGameObject);
				return true;
			}

			return false;
		}

		template<typename First, typename Second, typename ... Rest> // >=2 template parameters
		bool CheckComponents(GameObject* pGameObject)
		{
			if (pGameObject->GetComponent<First>())
			{
				CheckComponents<Second, Rest...>(pGameObject);
				return true;
			}

			return false;
		}

	public:
		int mTier;

	protected:
		std::vector<GameObject*> mGameObjects;
	};
}