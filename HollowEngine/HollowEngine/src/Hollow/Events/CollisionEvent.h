#pragma once
#include "Hollow/Events/GameEvent.h"

namespace Hollow {
	class GameObject;
	
	class HOLLOW_API CollisionEvent : public GameEvent
	{
	public:
		CollisionEvent() : GameEvent(COLLISION_EVENT), mObject1(nullptr), mObject2(nullptr) {}
		GameObject* mObject1;
		GameObject* mObject2;
	};
}