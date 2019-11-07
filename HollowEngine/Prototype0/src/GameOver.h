#pragma once
#include "Hollow/Events/GameEvent.h"

namespace Hollow {
	class GameObject;

	class GameOverEvent : public GameEvent
	{
	public:
		GameOverEvent() : GameEvent(GameEventType::GAME_OVER) {}
	};
}