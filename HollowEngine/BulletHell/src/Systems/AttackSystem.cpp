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
		// Send attack component to lua
		auto& lua = Hollow::ScriptingManager::Instance().lua;

		lua.new_usertype<Attack>("Attack",
			sol::constructors<Attack()>(),
			"baseAttackTime", &Attack::mBaseAttackTime,
			"currentAttackTime", &Attack::mCurrentAttackTime,
			"IsFired", &Attack::mIsFired
			);

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
		if (pGameObject->mType == (int)GameObjectType::PLAYER)
		{
			mpPlayerBody = pGameObject->GetComponent<Hollow::Body>();
		}
	}

	void AttackSystem::HandleBroadcastEvent(Hollow::GameEvent& event)
	{
	}

	void AttackSystem::PlayerAttackUpdate(Hollow::GameObject* pPlayer)
	{
		// Check buttons/axis directions and fire player attack script
		Attack* pAttack = pPlayer->GetComponent<Attack>();
		Hollow::Transform* pTransform = pPlayer->GetComponent<Hollow::Transform>();
		pAttack->mCurrentAttackTime += mDeltaTime;

		// Fire player lua script
		short rightHorizontalAxis = Hollow::InputManager::Instance().GetAxisValue(SDL_CONTROLLER_AXIS_RIGHTX);
		short rightVerticallAxis = Hollow::InputManager::Instance().GetAxisValue(SDL_CONTROLLER_AXIS_RIGHTY);
		auto& lua = Hollow::ScriptingManager::Instance().lua;

		// Send values to lua
		lua["attackPosition"] = pTransform->mPosition;
		lua["horizontalAxis"] = rightHorizontalAxis;
		lua["verticalAxis"] = rightVerticallAxis;
		lua["attack"] = pAttack;
		lua.script_file(pAttack->mScriptPath);

	}

	void AttackSystem::EnemyAttackUpdate(Hollow::GameObject* pEnemy)
	{
		Attack* pAttack = pEnemy->GetComponent<Attack>();
		pAttack->mCurrentAttackTime += mDeltaTime;
		// Fire attack and reset attack timer
		auto& lua = Hollow::ScriptingManager::Instance().lua;

		lua["attack"] = pAttack;
		lua["followObject"] = pEnemy;
		lua["followPosition"] = mpPlayerBody->mPosition;
		lua.script_file(pAttack->mScriptPath);
	}
}