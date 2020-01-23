#pragma once
#include "Hollow/Systems/System.h"

namespace BulletHell
{
	class MagicSystem : public Hollow::System
	{
		// Set priority to be before attack system
		REGISTERSYSTEM(MagicSystem, 2);
	public:
		void Update() override;
		void AddGameObject(Hollow::GameObject* pGameObject) override;

	};
}