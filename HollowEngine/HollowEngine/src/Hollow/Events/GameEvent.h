#pragma once
#include "Hollow/Core/Core.h"

namespace Hollow {

#define EVENT_CALLBACK(x) std::bind(&x,this,std::placeholders::_1)
	
	
	class HOLLOW_API GameEvent
	{
	public:
		GameEvent(int type) {
			mType = type;
		}
		virtual ~GameEvent() {}
		float mTimer = 0.0f;
		int mType; // identifier for event type

		GameObject* mpObject1;
		GameObject* mpObject2;
	};

}