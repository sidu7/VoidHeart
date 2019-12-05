#pragma once
#include "Hollow.h"
#include "Hollow/Components/Component.h"

namespace BulletHell
{
	class Attack : public Hollow::Component
	{
		REGISTERCOMPONENT(Attack);
	public:
		void Init();
		void Serialize(rapidjson::Value::Object data);
		void DeSerialize(rapidjson::Writer<rapidjson::StringBuffer>& writer);
		void Clear();
		void DebugDisplay();

	public:
		std::string mScriptPath;
		float mBaseAttackTime;
		float mCurrentAttackTime;
		bool mFireOnce;
		bool mIsFired;
	};
}