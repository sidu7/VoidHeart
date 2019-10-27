#include "PrototypeSystem.h"
#include "PrototypeComponent.h"

PrototypeSystem PrototypeSystem::instance;

void PrototypeSystem::Update()
{
	//HW_TRACE("Prototype system has {0} gameobjects", mGameObjects.size());
}

bool PrototypeSystem::HandleEvent(Hollow::GameEvent* gameEvent)
{
	HW_TRACE("EVENT TEST");
	return true;
}
void PrototypeSystem::AddGameObject(Hollow::GameObject* object)
{
	CheckComponents<PrototypeComponent>(object);
}
