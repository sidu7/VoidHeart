#include "AttackSystem.h"

#include "BulletHell/Components/Attack.h"

#include "Hollow/Components/Transform.h"

#include "Hollow/Managers/FrameRateController.h"
#include "Hollow/Managers/ScriptingManager.h"

namespace BulletHell
{
	AttackSystem AttackSystem::instance;

	void AttackSystem::Update()
	{
		float dt = Hollow::FrameRateController::Instance().GetFrameTime();
		for (unsigned int i = 0; i < mGameObjects.size(); ++i)
		{
			Attack* pAttack = mGameObjects[i]->GetComponent<Attack>();
			Hollow::Transform* pTransform = mGameObjects[i]->GetComponent<Hollow::Transform>();
			pAttack->mCurrentAttackTime += dt;
			if (pAttack->mCurrentAttackTime > pAttack->mBaseAttackTime)
			{
				// Fire attack and reset attack timer
				auto& lua = Hollow::ScriptingManager::Instance().lua;
				lua["attackPosition"] = pTransform->mPosition;
				lua.script_file(pAttack->mScriptPath);


				pAttack->mCurrentAttackTime = 0.0f;
			}

		}
	}

	void AttackSystem::AddGameObject(Hollow::GameObject* pGameObject)
	{
		CheckAllComponents<Attack, Hollow::Transform>(pGameObject);
	}
}