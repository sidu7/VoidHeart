#include "TestSystem.h"

#include <iostream>

#include "../Component/FakeComponent.h"

TestSystem::TestSystem()
{
}

TestSystem::~TestSystem()
{
}

void TestSystem::Update()
{
	for (GameObject* pGameObject : mGameObjects)
	{
		TestComponent* pTestComponent = pGameObject->GetComponent<TestComponent>();
		std::cout << pTestComponent->mValue << std::endl;
	}
}

void TestSystem::AddGameObject(GameObject* pGameObject)
{
	CheckComponents<TestComponent,FakeComponent>(pGameObject);
}
