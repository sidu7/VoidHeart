#pragma once
#include "Hollow/Systems/System.h"

namespace BulletHell
{
	class HandSystem : public Hollow::System
	{
		REGISTERSYSTEM(HandSystem, 4);
	public:
		void Init() override;
		void Update() override;
		void AddGameObject(Hollow::GameObject* pGameObject) override;
		void HandleBroadcastEvent(Hollow::GameEvent& event) override;
	};
}