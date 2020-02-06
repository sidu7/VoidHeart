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
			"IsFired", &Attack::mIsFired,
			"shouldAttack", &Attack::mShouldAttack,
			"target", &Attack::mpTarget
			);

		// Add get attack component to lua
		Hollow::ScriptingManager::Instance().mGameObjectType["GetAttack"] = &Hollow::GameObject::GetComponent<Attack>;
	}

	void AttackSystem::Update()
	{
		mDeltaTime = Hollow::FrameRateController::Instance().GetFrameTime();
		for (unsigned int i = 0; i < mGameObjects.size(); ++i)
		{
			// Run attack script for game object
			FireAttack(mGameObjects[i]);
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

	void AttackSystem::FireAttack(Hollow::GameObject* pGameObject)
	{
		// Check buttons/axis directions and fire player attack script
		Attack* pAttack = pGameObject->GetComponent<Attack>();
		pAttack->mCurrentAttackTime += mDeltaTime;

		// Fire player lua script
		auto& lua = Hollow::ScriptingManager::Instance().lua;
        lua["gameObject"] = pAttack->mpOwner;
		lua.script_file(pAttack->mScriptPath);
	}
}