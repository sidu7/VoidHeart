#include "DestroySystem.h"

#include "Components/DestroyTimer.h"

#include "Hollow/Managers/FrameRateController.h"
#include "Hollow/Managers/ScriptingManager.h"
#include "Hollow/Managers/GameObjectManager.h"

namespace BulletHell
{
	DestroySystem DestroySystem::instance;


	void DestroySystem::Init()
	{
		// Send Destroy component to lua
		auto& lua = Hollow::ScriptingManager::Instance().lua;

		lua.new_usertype<DestroyTimer>("DestroyTimer",
			sol::constructors<DestroyTimer()>(),
			"maxTime", &DestroyTimer::mMaxTime,
			"currentTime",&DestroyTimer::mCurrentTime
			);

		// Add get attack component to lua
		Hollow::ScriptingManager::Instance().mGameObjectType["GetDestroyTimer"] = &Hollow::GameObject::GetComponent<DestroyTimer>;
	}

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