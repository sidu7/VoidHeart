#include"hollowpch.h"
#include "Hollow/Managers/EventManager.h"
#include "Hollow/Managers/SystemManager.h"
#include "Hollow/Managers/FrameRateController.h"

namespace Hollow {
	bool EventManager::AddDelayedEvent(GameEvent* event, float delayTimer)
	{
		event->mTimer = delayTimer;
		mDelayedEvents.emplace_back(event);
		return true;
	}
	void EventManager::HandleEvents()
	{
		for (unsigned int i = 0; i < mCurrentEvents.size(); i++)
		{
			SystemManager::Instance().SendEventsToSystems(mCurrentEvents[i]);
		}

		mCurrentEvents.clear();

		for (unsigned int i = 0; i < mDelayedEvents.size(); i++)
		{
			if (mDelayedEvents[i]->mTimer <= 0.0f)
			{
				SystemManager::Instance().SendEventsToSystems(mDelayedEvents[i]);
				mDelayedEvents.erase(mDelayedEvents.begin() + i);
			}
			else
			{
				mDelayedEvents[i]->mTimer -= FrameRateController::Instance().GetFrameTime();
			}
		}
	}
	bool EventManager::AddEvent(GameEvent* event)
	{
		mCurrentEvents.emplace_back(event);
		return true;
	}
}