#pragma once
#include <Hollow.h>

#include "Hollow/Events/GameEvent.h"

#include "GameMetaData/GameEventType.h"

namespace BulletHell
{
	class CycleSpellEvent : public Hollow::GameEvent
	{
	public:
		CycleSpellEvent(std::string handString) : Hollow::GameEvent((int)GameEventType::CYCLE_SPELL),
		mHandString(handString){}

	public:
		std::string mHandString;
	};

}