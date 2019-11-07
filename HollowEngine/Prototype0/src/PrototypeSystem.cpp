#include "PrototypeSystem.h"
#include "PrototypeComponent.h"

#include "Hollow/Events/GameEvent.h"

#include "Hollow/Managers/EventManager.h"

PrototypeSystem PrototypeSystem::instance;

void PrototypeSystem::AddGameObject(Hollow::GameObject* object)
{
	CheckAllComponents<PrototypeComponent>(object);
}

void PrototypeSystem::TestEventHandling(Hollow::GameEvent* event)
{
	HW_TRACE("Event Collision");
}

void PrototypeSystem::Init()
{
	Hollow::EventManager::Instance().SubscribeEvent(Hollow::GameEvent::GAME_OVER,EVENT_CALLBACK(PrototypeSystem::TestEventHandling));
}

void PrototypeSystem::HandleBroadcastEvent(Hollow::GameEvent*)
{
	HW_TRACE("Broadcast EVENT TEST");
}

void PrototypeSystem::Update()
{
	//HW_TRACE("Prototype system has {0} gameobjects", mGameObjects.size());
}

