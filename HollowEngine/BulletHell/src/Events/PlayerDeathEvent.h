#pragma once
#include <Hollow.h>

#include "Hollow/Events/GameEvent.h"

#include "GameMetaData/GameEventType.h"

namespace BulletHell
{
	class PlayerDeathEvent : public Hollow::GameEvent
	{
	public:
		PlayerDeathEvent() : Hollow::GameEvent((int)GameEventType::PLAYER_DEATH) {}
	};

}