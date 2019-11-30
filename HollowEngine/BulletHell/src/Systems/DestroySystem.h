#pragma once
#include <Hollow.h>
#include "Hollow/Systems/System.h"

namespace BulletHell
{
	class DestroySystem : public Hollow::System
	{
		REGISTERSYSTEM(DestroySystem, 3);
	public:
		void Update();
		void AddGameObject(Hollow::GameObject* pGameObject);
	};
}