#include "HealthSystem.h"

#include "BulletHell/Components/Health.h"

#include "Hollow/Managers/GameObjectManager.h"

namespace BulletHell
{
	HealthSystem HealthSystem::instance;

	void HealthSystem::Update()
	{
		for (unsigned int i = 0; i < mGameObjects.size(); ++i)
		{
			Health* pHealth = mGameObjects[i]->GetComponent<Health>();
			if (pHealth->mHitPoints < 0)
			{
				pHealth->mIsAlive = false;
			}
			if (!pHealth->mIsAlive)
			{
				// Send event to destroy object
				Hollow::GameObjectManager::Instance().DeleteGameObject(mGameObjects[i]);
			}
		}
	}

	void HealthSystem::AddGameObject(Hollow::GameObject* pGameObject)
	{
		CheckAllComponents<Health>(pGameObject);
	}

}