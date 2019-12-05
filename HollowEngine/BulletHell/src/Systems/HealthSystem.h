#pragma once
#include "Hollow/Systems/System.h"

namespace BulletHell
{

	class HealthSystem : public Hollow::System
	{
		REGISTERSYSTEM(HealthSystem, 3)
	public:
		void Init() override;
		void Update() override;
		void AddGameObject(Hollow::GameObject* pGameObject) override;
		void HandleBroadcastEvent(Hollow::GameEvent& event) override;

	private:
		void OnBulletHitPlayer(Hollow::GameEvent& event);
		void OnBulletHitWall(Hollow::GameEvent& event);
		void OnPlayerBulletHitEnemy(Hollow::GameEvent& event);

		void HandleBulletDamage(Hollow::GameObject* pObjectHit, Hollow::GameObject* pBullet);
	};
}