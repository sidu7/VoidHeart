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
		HOLLOW_API void Init(rapidjson::Value::Object& data);
		HOLLOW_API void FireCollisionEvent(int, GameObject*, GameObject*);
		HOLLOW_API void AddDelayedEvent(GameEvent*, float);
		HOLLOW_API void BroadcastToSubscribers(GameEvent& event);
		HOLLOW_API void BroadcastEvent(GameEvent&);
		HOLLOW_API void Update();
		HOLLOW_API void SubscribeEvent(int eventType, std::function<void(GameEvent&)> callbackFunction);
		HOLLOW_API void CleanUp();
	private:
		std::unordered_map<int, int> mGameObjectPairEventMap;
		std::vector<GameEvent*> mDelayedEvents;
		std::unordered_map<int, std::vector<std::function<void(GameEvent&)>>> mEventsMap;
	};
}