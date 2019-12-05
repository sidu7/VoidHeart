#pragma once
#include "Hollow.h"
#include "Hollow/Components/Component.h"

namespace BulletHell
{
	class DestroyTimer : public Hollow::Component
	{
		REGISTERCOMPONENT(DestroyTimer);
	public:
		void Init();
		void Serialize(rapidjson::Value::Object data);
		void DeSerialize(rapidjson::Writer<rapidjson::StringBuffer>& writer);
		void Clear();
		void DebugDisplay();

	public:
		float mMaxTime;
		float mCurrentTime;
	};
}