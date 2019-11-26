#include "PrototypeSystem.h"
#include "PrototypeComponent.h"

#include "Hollow/Events/GameEvent.h"

#include "Hollow/Managers/EventManager.h"

#include "Hollow/Components/Animator.h"
#include "Hollow/Managers/GameObjectManager.h"
#include "GameMetaData/GameEventType.h"

PrototypeSystem PrototypeSystem::instance;

void PrototypeSystem::AddGameObject(Hollow::GameObject* object)
{
}

void PrototypeSystem::TestEventHandling(Hollow::GameEvent& event)
{
	//HW_TRACE("Event Collision");
	Hollow::GameObjectManager::Instance().DeleteGameObject(event.mObject1);
}

void PrototypeSystem::Init()
{
	Hollow::EventManager::Instance().SubscribeEvent((int)GameEventType::GAME_OVER,EVENT_CALLBACK(PrototypeSystem::TestEventHandling));
	Hollow::EventManager::Instance().SubscribeEvent((int)GameEventType::ON_ROCK_HIT,EVENT_CALLBACK(PrototypeSystem::TestEventHandling));
}

void PrototypeSystem::HandleBroadcastEvent(Hollow::GameEvent& event)
{
	//HW_TRACE("Broadcast EVENT TEST");
	if (event.mType == (int)GameEventType::ON_POINT_COLLECTED)
	{
		Hollow::GameObjectManager::Instance().DeleteGameObject(event.mObject1);
		Hollow::GameObjectManager::Instance().DeleteGameObject(event.mObject2);
	}
}

void PrototypeSystem::Update()
{
}

