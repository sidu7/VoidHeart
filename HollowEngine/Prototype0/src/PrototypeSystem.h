#pragma once
#include <Hollow.h>
#include "Hollow/Systems/System.h"

class PrototypeSystem : public Hollow::System
{
	REGISTERSYSTEM(PrototypeSystem,2)
public:
	~PrototypeSystem() {}
	bool HandleEvent(Hollow::GameEvent*);
	void Update();
	void AddGameObject(Hollow::GameObject* object);
};