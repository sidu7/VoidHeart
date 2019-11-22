#pragma once
#include "Hollow/Common.h"
#include "Hollow/Events/GameEvent.h"

namespace Hollow
{
	class GameObject;
	
	class EventManager
	{
		SINGLETON(EventManager)
	public:
		HOLLOW_API void Init();
		HOLLOW_API void BroadcastToSubscribers(GameEvent* event);
		HOLLOW_API void BroadcastEvent(GameEvent*);
		HOLLOW_API void AddDelayedEvent(GameEvent*, float);
		HOLLOW_API void Update();
		HOLLOW_API void SubscribeEvent(GameEvent::GameEventType eventType, std::function<void(GameEvent*)> callbackFunction);
		HOLLOW_API void CleanUp();
	private:
		std::vector<GameEvent*> mDelayedEvents;
		std::unordered_map<GameEvent::GameEventType, std::vector<std::function<void(GameEvent*)>>> mEventsMap;
		std::unordered_map<std::string, GameEvent::GameEventType> mEventsEnumMap;
	};
}