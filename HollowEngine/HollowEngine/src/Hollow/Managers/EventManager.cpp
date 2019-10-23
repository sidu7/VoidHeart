#include"hollowpch.h"
#include "Hollow/Managers/EventManager.h"
#include "Hollow/Managers/SystemManager.h"
#include "Hollow/Managers/FrameRateController.h"

namespace Hollow {
	bool EventManager::AddDelayedEvent(GameEvent event, Uint32 delayTimer)
	{
		event.mTimer = delayTimer;
		mDelayedEvents.push_back(event);
		return true;
	}
	void EventManager::HandleEvents()
	{
		for (unsigned int i = 0; i < mCurrentEvents.size(); i++)
		{
			SystemManager::Instance().AddEventsToSystems(mCurrentEvents[i]);
		}
	
		mCurrentEvents.clear();

		for (unsigned int i = 0; i < mDelayedEvents.size(); i++)
		{
			if (mDelayedEvents[i].mTimer <=0)
			{
				SystemManager::Instance().AddEventsToSystems(mCurrentEvents[i]);
				mDelayedEvents.erase(mDelayedEvents.begin()+i);
			}
			else
				mDelayedEvents[i].mTimer-=FrameRateController::Instance().GetFrameTime();
		}
	}
	bool EventManager::AddEvent(GameEvent event)
	{
		mCurrentEvents.push_back(event);
		return true;
	}
}