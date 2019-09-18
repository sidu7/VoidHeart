#pragma once

#include "System.h"

class TestSystem : public System<TestSystem>
{	
public:	
	TestSystem();
	virtual ~TestSystem();

	void Update();
	void AddGameObject(GameObject* pGameObject);
};