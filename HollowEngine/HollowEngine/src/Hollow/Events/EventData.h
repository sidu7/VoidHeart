#pragma once
namespace Hollow {
	class GameEvent
	{
	public:
		GameEvent(std::string name) {
			mEventType = name;
		}
		float mTimer = 0.0f;
		std::string mEventType;
	};

}