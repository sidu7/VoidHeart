#pragma once
#include"Hollow/Events/EventData.h"
#include"Hollow/Events/CollisionEvent.h"
namespace Hollow {
	class HOLLOW_API EventManager {
		SINGLETON(EventManager)
	public:
		bool AddEvent(GameEvent);
		bool AddDelayedEvent(GameEvent, Uint32);
		void HandleEvents();
	private:
		std::vector<GameEvent> mCurrentEvents;
		std::vector<GameEvent> mDelayedEvents;
	};
}