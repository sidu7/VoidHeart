#pragma once
namespace Hollow {
	class GameEvent
	{
	public:
		GameEvent(std::string name) {
			mEventType = name;
		}
		Uint32 mTimer = 0;
		std::string mEventType;
	};

}