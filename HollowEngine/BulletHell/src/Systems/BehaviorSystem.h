#pragma once
#include "Hollow/Systems/System.h"

namespace BulletHell
{
	class BehaviorSystem : public Hollow::System
	{
		REGISTERSYSTEM(BehaviorSystem, 3)
	public:
		void Init() override;
		void Update() override;
		void AddGameObject(Hollow::GameObject* pGameObject) override;
		void HandleBroadcastEvent(Hollow::GameEvent& event) override;
	};
}