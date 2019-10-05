#pragma once
#include <Hollow.h>
#include "Hollow/Systems/System.h"

class PrototypeSystem : public Hollow::System
{
	REGISTERSYSTEM(PrototypeSystem,2)
public:
	~PrototypeSystem() {}
	void Update(Hollow::GameObject* gameobject);
	void AddGameObject(Hollow::GameObject* object);
};