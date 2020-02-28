#pragma once
#include <Hollow.h>

#include "Hollow/Events/GameEvent.h"

#include "GameMetaData/GameEventType.h"
#include "GameMetaData/GameObjectType.h"

namespace BulletHell
{
	class Pickup;
	
	class PickupTimedEvent : public Hollow::GameEvent
	{
	public:
		PickupTimedEvent(Pickup* p) : Hollow::GameEvent((int)GameEventType::ON_PICKUP_EFFECT_END), mpPickup(*p) {}
	public:
		Pickup mpPickup;
	};

}