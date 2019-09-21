#include <hollowpch.h>

#include "TempSystem.h"
#include "Hollow/Components/TestComponent.h"

namespace Hollow
{
	TempSystem TempSystem::instance;

	void TempSystem::Update()
	{
		HW_CORE_TRACE("Test system has {0} gameobjects", mGameObjects.size());
	}

	void TempSystem::AddGameObject(GameObject* object)
	{
		CheckComponents<TestComponent>(object);
	}
}