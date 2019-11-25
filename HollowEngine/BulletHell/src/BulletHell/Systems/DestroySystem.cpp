#include "DestroySystem.h"

#include "BulletHell/Components/DestroyTimer.h"

#include "Hollow/Managers/FrameRateController.h"
#include "Hollow/Managers/GameObjectManager.h"

namespace BulletHell
{
	DestroySystem DestroySystem::instance;

	void DestroySystem::Update()
	{
		// Update timer
		float dt = Hollow::FrameRateController::Instance().GetFrameTime();
		for (unsigned int i = 0; i < mGameObjects.size(); ++i)
		{
			DestroyTimer* pDestroyTimer = mGameObjects[i]->GetComponent<DestroyTimer>();
			pDestroyTimer->mCurrentTime += dt;
			if (pDestroyTimer->mCurrentTime > pDestroyTimer->mMaxTime)
			{
				// Send event to destroy object
				Hollow::GameObjectManager::Instance().DeleteGameObject(mGameObjects[i]);
			}
		}

	}

	void DestroySystem::AddGameObject(Hollow::GameObject* pGameObject)
	{
		CheckAllComponents<DestroyTimer>(pGameObject);
	}
}