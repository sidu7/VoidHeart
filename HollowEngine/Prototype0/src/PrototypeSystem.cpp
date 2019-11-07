#include "PrototypeSystem.h"
#include "PrototypeComponent.h"

#include "Hollow/Events/GameEvent.h"

#include "Hollow/Managers/EventManager.h"

#include "Hollow/Components/Animator.h"

PrototypeSystem PrototypeSystem::instance;

void PrototypeSystem::AddGameObject(Hollow::GameObject* object)
{
	CheckAllComponents<Hollow::Animator>(object);
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
	Hollow::Animator* anim = mGameObjects[0]->GetComponent<Hollow::Animator>();
	HW_TRACE("Animator state {0}", anim->mCurrentState);
}

