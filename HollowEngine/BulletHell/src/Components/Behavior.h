#pragma once
#include "Hollow/Components/Component.h"

namespace BulletHell
{
	class Behavior : public Hollow::Component
	{
		REGISTERCOMPONENT(Behavior);
	public:
		void Init();
		void Serialize(rapidjson::Value::Object data);
		void DeSerialize(rapidjson::Writer<rapidjson::StringBuffer>& writer);
		void Clear();
		void DebugDisplay();

	public:
		std::string mCurrentBehavior;
		std::unordered_map<std::string, std::string> mBehaviorStates;
		std::vector<std::pair<int, std::string>> mThreshold;
	};
}