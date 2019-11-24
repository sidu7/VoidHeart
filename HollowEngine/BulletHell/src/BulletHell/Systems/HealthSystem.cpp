#include "HealthSystem.h"

#include "BulletHell/Components/Health.h"

namespace BulletHell
{
	HealthSystem HealthSystem::instance;

	void HealthSystem::Update()
	{
	}

	void HealthSystem::AddGameObject(Hollow::GameObject* pGameObject)
	{
		if (CheckAllComponents<Health>(pGameObject))
		{

		}
	}

}