#pragma once
#include <Hollow.h>
#include "Hollow/Systems/System.h"

class PrototypeSystem : public Hollow::System
{
	REGISTERSYSTEM(PrototypeSystem)
public:
	~PrototypeSystem() {}
	void Update();
	void AddGameObject(Hollow::GameObject* object);
};