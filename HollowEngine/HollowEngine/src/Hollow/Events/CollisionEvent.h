#pragma once
#include <string>
#include "Hollow/Core/Core.h"
#include "Hollow/Events/EventData.h"
#include "Hollow/Core/GameObject.h"

namespace Hollow {
	class CollisionEvent : public GameEvent
	{
		REGISTEREVENT(CollisionEvent);
		GameObject* mObject1;
		GameObject* mObject2;
	};
}