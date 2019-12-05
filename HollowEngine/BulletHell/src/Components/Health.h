#pragma once
#include "Hollow.h"
#include "Hollow/Components/Component.h"

namespace BulletHell
{
	class Health : public Hollow::Component
	{
		REGISTERCOMPONENT(Health);
	public:
		void Init();
		void Serialize(rapidjson::Value::Object data);
		void DeSerialize(rapidjson::Writer<rapidjson::StringBuffer>& writer);
		void Clear();
		void DebugDisplay();

	public:
		int mHitPoints;
		bool mIsAlive;
		bool mInvincible;
		float mInvincibleTime;
		float mCurrentInvincibleTime;
	};
}
