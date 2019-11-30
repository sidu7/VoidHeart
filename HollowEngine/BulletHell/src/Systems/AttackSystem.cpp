#include <Hollow.h>
#include "AttackSystem.h"

#include "Hollow/Components/Transform.h"

#include "Hollow/Managers/FrameRateController.h"
#include "Hollow/Managers/ScriptingManager.h"
#include "Hollow/Managers/InputManager.h"

#include "GameMetaData/GameEventType.h"
#include "GameMetaData/GameObjectType.h"

#include "Components/Attack.h"

namespace BulletHell
{
	AttackSystem AttackSystem::instance;

	void AttackSystem::Init()
	{
	}

	void AttackSystem::Update()
	{
		mDeltaTime = Hollow::FrameRateController::Instance().GetFrameTime();
		for (unsigned int i = 0; i < mGameObjects.size(); ++i)
		{
			// Handle player attack script
			if (mGameObjects[i]->mType == (int)GameObjectType::PLAYER)
			{
				PlayerAttackUpdate(mGameObjects[i]);
			}
			// Handle enemy attack script for now
			else
			{
				EnemyAttackUpdate(mGameObjects[i]);
			}
		}
	}

	void AttackSystem::AddGameObject(Hollow::GameObject* pGameObject)
	{
		CheckAllComponents<Attack, Hollow::Transform>(pGameObject);
	}

	void AttackSystem::HandleBroadcastEvent(Hollow::GameEvent& event)
	{
	}

	void AttackSystem::PlayerAttackUpdate(Hollow::GameObject* pPlayer)
	{
		// Check buttons/axis directions and fire player attack script
		Attack* pAttack = pPlayer->GetComponent<Attack>();
		Hollow::Transform* pTransform = pPlayer->GetComponent<Hollow::Transform>();

		// Fire player lua script
		if (Hollow::InputManager::Instance().IsControllerButtonTriggered(SDL_CONTROLLER_BUTTON_A))
		{
			auto& lua = Hollow::ScriptingManager::Instance().lua;
			lua["attackPosition"] = pTransform->mPosition;
			lua.script_file(pAttack->mScriptPath);
		}
	}

	void AttackSystem::EnemyAttackUpdate(Hollow::GameObject* pEnemy)
	{
		Attack* pAttack = pEnemy->GetComponent<Attack>();
		Hollow::Transform* pTransform = pEnemy->GetComponent<Hollow::Transform>();
		pAttack->mCurrentAttackTime += mDeltaTime;
		if (pAttack->mCurrentAttackTime > pAttack->mBaseAttackTime)
		{
			// Fire attack and reset attack timer
			//auto& lua = Hollow::ScriptingManager::Instance().lua;

			//lua["attackPosition"] = pTransform->mPosition;
			//lua.script_file(pAttack->mScriptPath);

			//pAttack->mCurrentAttackTime = 0.0f;
		}
	}
}