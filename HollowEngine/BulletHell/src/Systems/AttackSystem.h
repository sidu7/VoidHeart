#pragma once
#include "Hollow/Systems/System.h"

namespace BulletHell
{
	class AttackSystem : public Hollow::System
	{
		REGISTERSYSTEM(AttackSystem, 3);
	public:
		void Init() override;
		void Update() override;
		void AddGameObject(Hollow::GameObject* pGameObject) override;
		void HandleBroadcastEvent(Hollow::GameEvent& event) override;

	private:
		void PlayerAttackUpdate(Hollow::GameObject* pPlayer);
		void EnemyAttackUpdate(Hollow::GameObject* pEnemy);

	private:
		float mDeltaTime;
	};
}