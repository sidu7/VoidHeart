#pragma once
#include <Hollow.h>
#include "Hollow/Systems/System.h"

namespace BulletHell
{
	class AttackSystem : public Hollow::System
	{
		REGISTERSYSTEM(AttackSystem, 3);
	public:
		void Update();
		void AddGameObject(Hollow::GameObject* pGameObject);
	};
}