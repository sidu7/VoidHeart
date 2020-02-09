#pragma once
#include <Hollow.h>

#include "Hollow/Events/GameEvent.h"

#include "GameMetaData/GameEventType.h"
#include "GameMetaData/GameObjectType.h"

namespace BulletHell
{
	class DeathEvent : public Hollow::GameEvent
	{
	public:
		DeathEvent(int type) : Hollow::GameEvent((int)GameEventType::DEATH), mType(type) {}
	public:
		int mType;
	};

}