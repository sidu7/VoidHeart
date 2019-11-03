#pragma once
#include "Hollow/Core/Core.h"

namespace Hollow {

#define EVENT_CALLBACK(x) std::bind(&x,this,std::placeholders::_1)
	
	enum HOLLOW_API GameEventType
	{
		COLLISION_EVENT,
		NUM
	};
	
	class HOLLOW_API GameEvent
	{
	public:
		GameEvent(GameEventType type) {
			mType = type;
		}
		float mTimer = 0.0f;
		GameEventType mType;
	};

}