#include <hollowpch.h>
#include "EventManager.h"

#include "Hollow/Events/GameEvent.h"

#include "Hollow/Managers/SystemManager.h"
#include "Hollow/Managers/FrameRateController.h"

namespace Hollow {
	
	void EventManager::AddDelayedEvent(GameEvent* event, float delayTimer)
	{
		event->mTimer = delayTimer;
		mDelayedEvents.emplace_back(event);
	}
	
	void EventManager::Update()
	{
		for (unsigned int i = 0; i < mDelayedEvents.size(); i++)
		{
			if (mDelayedEvents[i]->mTimer <= 0.0f)
			{
				SystemManager::Instance().BroadcastEventToSystems(mDelayedEvents[i]);
				GameEvent* event = mDelayedEvents[i];
				mDelayedEvents.erase(mDelayedEvents.begin() + i);
				delete event;
			}
			else
			{
				mDelayedEvents[i]->mTimer -= FrameRateController::Instance().GetFrameTime();
			}
		}
	}

	void EventManager::SubscribeEvent(GameEventType eventType, std::function<void(GameEvent*)> callbackFunction)
	{
		mEventsMap[eventType].emplace_back(callbackFunction);
	}

	void EventManager::CleanUp()
	{
		mEventsMap.clear();
		std::for_each(mDelayedEvents.begin(), mDelayedEvents.end(), [](GameEvent* event) {delete event; });
		mDelayedEvents.clear();
	}

	void EventManager::BroadcastToSubscribers(GameEvent* event)
	{
		if(mEventsMap.find(event->mType) != mEventsMap.end())
		{
			auto& list = mEventsMap[event->mType];
			for(auto function : list)
			{
				function(event);
			}
		}
		else
		{
			HW_CORE_ERROR("EventType not found");
		}
	}

	void EventManager::BroadcastEvent(GameEvent* event)
	{
		SystemManager::Instance().BroadcastEventToSystems(event);
	}
}