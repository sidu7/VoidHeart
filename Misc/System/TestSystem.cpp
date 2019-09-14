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
	// Check for components then add to list
	if (pGameObject->GetComponent<TestComponent>())
	{
		mGameObjects.push_back(pGameObject);
	}
	if (pGameObject->GetComponent<FakeComponent>())
	{
		std::cout << "Have a fake component" << std::endl;
	}
}
