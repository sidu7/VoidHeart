#pragma once
#include "Hollow/Systems/System.h"

namespace BulletHell
{
	class MovementSystem : public Hollow::System
	{
		REGISTERSYSTEM(MovementSystem, 2);
	public:
		void Update();
		void AddGameObject(Hollow::GameObject* pGameObject);
	};
}