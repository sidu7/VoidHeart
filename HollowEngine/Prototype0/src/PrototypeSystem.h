#pragma once
#include <Hollow.h>
#include "Hollow/Systems/System.h"

class Hollow::GameEvent;

class PrototypeSystem : public Hollow::System
{
	REGISTERSYSTEM(PrototypeSystem,2)
public:
	~PrototypeSystem() {}
	void Init() override;
	void HandleBroadcastEvent(Hollow::GameEvent&) override;
	void Update();
	void AddGameObject(Hollow::GameObject* object);
	void TestEventHandling(Hollow::GameEvent& event);
};