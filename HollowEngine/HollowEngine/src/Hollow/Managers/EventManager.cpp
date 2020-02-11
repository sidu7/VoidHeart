#include <hollowpch.h>
#include "EventManager.h"

#include "Hollow/Managers/SystemManager.h"
#include "Hollow/Managers/FrameRateController.h"
#include "Hollow/Core/GameMetaData.h"

namespace Hollow {
	
	void EventManager::Init(rapidjson::Value::Object& data)
	{
		// Parse Event data and setup the map of object pair to event type map
		std::ifstream file(std::string(data["EventData"].GetString()));
		std::string line;

		if (file.is_open())
		{
			while (getline(file, line))
			{
				std::istringstream iss(line);
				std::vector<std::string> results(std::istream_iterator<std::string>{iss},
					std::istream_iterator<std::string>());

				int id1 = GameMetaData::Instance().mMapOfGameObjectTypes[results[0]];
				int id2 = GameMetaData::Instance().mMapOfGameObjectTypes[results[1]];
				mGameObjectPairEventMap[BIT(id1) | BIT(id2)] = GameMetaData::Instance().mMapOfGameEventTypes[results[2]];
			}
			file.close();
		}
	}

	void EventManager::FireCollisionEvent(int eventId, GameObject* go1, GameObject* go2)
	{
		if(mGameObjectPairEventMap.count(eventId) <= 0)
		{
			return;
		}
		
		GameEvent te(mGameObjectPairEventMap[eventId]);
		te.mpObject1 = go1;
		te.mpObject2 = go2;

		if(mEventsMap.find(te.mType) != mEventsMap.end())
		{
			BroadcastToSubscribers(te);
		}
		else
		{
			BroadcastEvent(te); // sends to all systems
		}
	}

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
				SystemManager::Instance().BroadcastEventToSystems(*mDelayedEvents[i]);
				GameEvent* event = mDelayedEvents[i];
				BroadcastToSubscribers(*event);
				mDelayedEvents.erase(mDelayedEvents.begin() + i);
				delete event;
			}
			else
			{
				mDelayedEvents[i]->mTimer -= FrameRateController::Instance().GetFrameTime();
			}
		}
	}

	void EventManager::SubscribeEvent(int eventType, std::function<void(GameEvent&)> callbackFunction)
	{
		mEventsMap[eventType].emplace_back(callbackFunction);
	}

	void EventManager::CleanUp()
	{
		mEventsMap.clear();
		std::for_each(mDelayedEvents.begin(), mDelayedEvents.end(), [](GameEvent* event) {delete event; });
		mDelayedEvents.clear();
	}

	void EventManager::BroadcastToSubscribers(GameEvent& event)
	{
		if(mEventsMap.find(event.mType) != mEventsMap.end())
		{
			auto& list = mEventsMap[event.mType];
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

	void EventManager::BroadcastEvent(GameEvent& event)
	{
		SystemManager::Instance().BroadcastEventToSystems(event);
	}
}
