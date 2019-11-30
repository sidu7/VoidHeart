#include <Hollow.h>
#include "BehaviorSystem.h"

#include "Hollow/Components/Body.h"

#include "Hollow/Managers/ScriptingManager.h"

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

			// Get threshold values from behavior
			for (auto& thresholdPair : pBehavior->mThreshold)
			{
				if (pHealth->mHitPoints >= thresholdPair.first)
				{
					pBehavior->mCurrentBehavior = thresholdPair.second;
					break;
				}
			}

			// Update position based on script
			{
				Hollow::Body* pBody = mGameObjects[i]->GetComponent<Hollow::Body>();
				Attack* pAttack = mGameObjects[i]->GetComponent<Attack>();

				auto& lua = Hollow::ScriptingManager::Instance().lua;
				lua["enemyBody"] = pBody;
				lua["currentAttackTime"] = pAttack->mCurrentAttackTime;
				lua["baseAttackTime"] = pAttack->mBaseAttackTime;
				lua["attackPosition"] = pBody->mPosition;
				std::string filepath = "Resources/Scripts/" + pBehavior->mBehaviorStates[pBehavior->mCurrentBehavior];
				lua.script_file(filepath);
				pAttack->mCurrentAttackTime = lua["currentAttackTime"];
			}
		}
	}

	void BehaviorSystem::AddGameObject(Hollow::GameObject* pGameObject)
	{
		CheckAllComponents<Behavior, Hollow::Body, Attack, Health>(pGameObject);
	}
	
	void BehaviorSystem::HandleBroadcastEvent(Hollow::GameEvent& event)
	{
	}
}