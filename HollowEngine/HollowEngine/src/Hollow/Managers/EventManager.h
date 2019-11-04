#pragma once

namespace Hollow
{
	class GameEvent;
	enum GameEventType;
	class GameObject;
	
	class HOLLOW_API EventManager
	{
		SINGLETON(EventManager)
	public:
		void BroadcastToSubscribers(GameEvent* event);
		void BroadcastEvent(GameEvent*);
		void AddDelayedEvent(GameEvent*, float);
		void Update();
		void SubscribeEvent(GameEventType eventType, std::function<void(GameEvent*)> callbackFunction);
		void CleanUp();
	private:
		std::vector<GameEvent*> mDelayedEvents;
		std::unordered_map<GameEventType, std::vector<std::function<void(GameEvent*)>>> mEventsMap;
	};
}