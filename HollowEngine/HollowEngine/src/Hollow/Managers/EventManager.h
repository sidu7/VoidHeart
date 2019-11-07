#pragma once
#include "Hollow/Events/GameEvent.h"

namespace Hollow
{
	class GameObject;
	
	class HOLLOW_API EventManager
	{
		SINGLETON(EventManager)
	public:
		void Init();
		void BroadcastToSubscribers(GameEvent* event);
		void BroadcastEvent(GameEvent*);
		void AddDelayedEvent(GameEvent*, float);
		void Update();
		void SubscribeEvent(GameEvent::GameEventType eventType, std::function<void(GameEvent*)> callbackFunction);
		void CleanUp();
	private:
		std::vector<GameEvent*> mDelayedEvents;
		std::unordered_map<GameEvent::GameEventType, std::vector<std::function<void(GameEvent*)>>> mEventsMap;
		std::unordered_map<std::string, GameEvent::GameEventType> mEventsEnumMap;
	};
}