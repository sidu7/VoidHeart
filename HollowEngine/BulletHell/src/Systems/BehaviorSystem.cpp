#include <Hollow.h>
#include "BehaviorSystem.h"

#include "Hollow/Components/Body.h"

#include "Hollow/Managers/ScriptingManager.h"
#include "Hollow/Managers/FrameRateController.h"

#include "GameMetaData/GameObjectType.h"

#include "Components/Behavior.h"
#include "Components/Attack.h"
#include "Components/Health.h"

namespace BulletHell
{

	BehaviorSystem BehaviorSystem::instance;

	void BehaviorSystem::Init()
	{
	}

	void BehaviorSystem::Update()
	{
		for (unsigned int i = 0; i < mGameObjects.size(); ++i)
		{
			Behavior* pBehavior = mGameObjects[i]->GetComponent<Behavior>();
			Health* pHealth = mGameObjects[i]->GetComponent<Health>();

			mDeltaTime = Hollow::FrameRateController::Instance().GetFrameTime();
			// Get threshold values from behavior
			for (auto& thresholdPair : pBehavior->mThreshold)
			{
				if (pHealth->mHitPoints >= thresholdPair.first)
				{
					if (pBehavior->mCurrentBehavior != thresholdPair.second)
					{
						// TODO: Magic number transition time for now
						pBehavior->mTransitionTime = 5.0f;
						// TODO: Move this elsewhere
						pHealth->mInvincible = true;
						pHealth->mCurrentInvincibleTime = 0.0f;
					}
					pBehavior->mCurrentBehavior = thresholdPair.second;
					break;
				}
			}
			
			// May need to put in a safety check for overflow
			pBehavior->mTransitionTime -= mDeltaTime;

			// Update position based on script
			{
				Hollow::Body* pBody = mGameObjects[i]->GetComponent<Hollow::Body>();
				Attack* pAttack = mGameObjects[i]->GetComponent<Attack>();

				auto& lua = Hollow::ScriptingManager::Instance().lua;
				lua["enemyBody"] = pBody;
				lua["currentAttackTime"] = pAttack->mCurrentAttackTime;
				lua["baseAttackTime"] = pAttack->mBaseAttackTime;
				lua["attackPosition"] = pBody->mPosition;
				lua["transitionTime"] = pBehavior->mTransitionTime;
				lua["playerBody"] = mpPlayerBody->mPosition;
				std::string filepath = "Resources/Scripts/" + pBehavior->mBehaviorStates[pBehavior->mCurrentBehavior];
				lua.script_file(filepath);
				pAttack->mCurrentAttackTime = lua["currentAttackTime"];
			}
		}
	}

	void BehaviorSystem::AddGameObject(Hollow::GameObject* pGameObject)
	{
		CheckAllComponents<Behavior, Hollow::Body, Attack, Health>(pGameObject);
		if (pGameObject->mType == (int)GameObjectType::PLAYER)
		{
			mpPlayerBody = pGameObject->GetComponent<Hollow::Body>();
		}
	}
	
	void BehaviorSystem::HandleBroadcastEvent(Hollow::GameEvent& event)
	{
	}
}