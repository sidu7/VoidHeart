#pragma once

#include <vector>
#include "../Core/GameObject.h"

class System
{
public:
	System() {}
	virtual ~System() {}
	virtual void Update() = 0;
	virtual void AddGameObject(GameObject* pGameObject) = 0;

protected:
	std::vector<GameObject*> mGameObjects;
	std::vector<Component*> mComponents;
};