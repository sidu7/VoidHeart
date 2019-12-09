#pragma once
#include "Hollow/Common.h"
#include "Hollow/Events/GameEvent.h"

namespace Hollow
{
	class GameObject;
	
	/// <summary>
	/// Class Event Manager.
	/// </summary>
	class EventManager
	{
		SINGLETON(EventManager)
	public:
		/// <summary>
		/// Initializes manager data.
		/// </summary>
		HOLLOW_API void Init(rapidjson::Value::Object& data);
		
		/// <summary>
		/// Fires the collision event.
		/// </summary>
		/// <param name="eventName">Name of the event.</param>
		/// <param name="gameObject1">The game object 1.</param>
		/// <param name="gameObject2">The game object 2.</param>
		HOLLOW_API void FireCollisionEvent(int eventName, GameObject* gameObject1, GameObject* gameObject2);
		
		/// <summary>
		/// Adds delayed event.
		/// </summary>
		/// <param name="eventName">Name of the event.</param>
		/// <param name="delayTime">The delay time.</param>
		HOLLOW_API void AddDelayedEvent(GameEvent* eventName, float delayTime);
		
		/// <summary>
		/// Broadcasts event to subscribers.
		/// </summary>
		/// <param name="event">The event object.</param>
		HOLLOW_API void BroadcastToSubscribers(GameEvent& event);
		
		/// <summary>
		/// Broadcasts the event to all systems.
		/// </summary>
		/// <param name="event">The event object.</param>
		HOLLOW_API void BroadcastEvent(GameEvent& event);
		
		/// <summary>
		/// Updates manager data.
		/// </summary>
		HOLLOW_API void Update();
		
		/// <summary>
		/// Subscribes a function to the event.
		/// </summary>
		/// <param name="eventType">Type of the event.</param>
		/// <param name="callbackFunction">The callback function.</param>
		HOLLOW_API void SubscribeEvent(int eventType, std::function<void(GameEvent&)> callbackFunction);
		
		/// <summary>
		/// Cleans manager data.
		/// </summary>
		HOLLOW_API void CleanUp();
	private:
		std::unordered_map<int, int> mGameObjectPairEventMap;
		std::vector<GameEvent*> mDelayedEvents;
		std::unordered_map<int, std::vector<std::function<void(GameEvent&)>>> mEventsMap;
	};
}