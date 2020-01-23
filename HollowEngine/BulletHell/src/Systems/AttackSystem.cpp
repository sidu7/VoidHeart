#include <Hollow.h>
#include "AttackSystem.h"

#include "Hollow/Components/Transform.h"
#include "Hollow/Components/Material.h"

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

		// Temporary, find a way to get game component in engine
		lua.new_usertype<Hollow::GameObject>("GameObject",
			sol::constructors<Hollow::GameObject()>(),
			"GetBody", &Hollow::GameObject::GetComponent<Hollow::Body>,
			"GetTransform", &Hollow::GameObject::GetComponent<Hollow::Transform>,
			"GetMaterial", &Hollow::GameObject::GetComponent<Hollow::Material>,
			"GetAttack", &Hollow::GameObject::GetComponent<Attack>
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
		pAttack->mCurrentAttackTime += mDeltaTime;

		// Fire player lua script
		auto& lua = Hollow::ScriptingManager::Instance().lua;
		lua.script_file(pAttack->mScriptPath);

	}

	void AttackSystem::EnemyAttackUpdate(Hollow::GameObject* pEnemy)
	{
		Attack* pAttack = pEnemy->GetComponent<Attack>();
		pAttack->mCurrentAttackTime += mDeltaTime;
		// Fire attack and reset attack timer
		if (mpPlayerBody != nullptr)
		{
			auto& lua = Hollow::ScriptingManager::Instance().lua;

			lua["attack"] = pAttack;
			lua["followObject"] = pEnemy;
			lua["followPosition"] = mpPlayerBody->mPosition;
			lua.script_file(pAttack->mScriptPath);
		}
	}
}