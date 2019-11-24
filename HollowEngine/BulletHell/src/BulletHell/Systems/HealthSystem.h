#pragma once
#include <Hollow.h>
#include "Hollow/Systems/System.h"

namespace BulletHell
{
	class HealthSystem : public Hollow::System
	{
		REGISTERSYSTEM(HealthSystem, 3)
	public:
		void Update();
		void AddGameObject(Hollow::GameObject* pGameObject);
	};
}