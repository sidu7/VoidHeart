#include <iostream>

#include "System/TestSystem.h"
#include "Core/GameObject.h"

int main()
{

	TestSystem* pTestSystem = new TestSystem();
	
	// Game object manager
	{
		GameObject* pGameObject = new GameObject();
		pGameObject->AddComponent<TestComponent>();
		//for(all systems)
		pTestSystem->AddGameObject(pGameObject);
	}

	while (true)
	{
		// Update systems
		pTestSystem->Update();
	}

	return 0;
}