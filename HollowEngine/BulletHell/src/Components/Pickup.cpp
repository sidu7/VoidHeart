#include "Pickup.h"
namespace BulletHell
{
	Pickup Pickup::instance;

	void BulletHell::Pickup::Init()
	{
		// by default all pickups act instantaneously
		mEffectTime = 0.0f;
		mBuffValue = 0.0f;
	}

	void BulletHell::Pickup::Serialize(rapidjson::Value::Object data)
	{
		if (data.HasMember("PickupType"))
		{
			mPickupType = data["PickupType"].GetInt();
		}
		if (data.HasMember("BuffValue"))
		{
			mBuffValue = data["BuffValue"].GetFloat();
		}
		if (data.HasMember("EffectTime"))
		{
			mEffectTime = data["EffectTime"].GetFloat();
		}
	}

	void BulletHell::Pickup::DeSerialize(rapidjson::Writer<rapidjson::StringBuffer>& writer)
	{
		Hollow::JSONHelper::Write<int>("PickupType", mPickupType, writer);
		Hollow::JSONHelper::Write<float>("BuffValue", mBuffValue, writer);
		Hollow::JSONHelper::Write<float>("EffectTime", mEffectTime, writer);
	}

	void BulletHell::Pickup::Clear()
	{
	}

	void BulletHell::Pickup::DebugDisplay()
	{
		ImGui::InputInt("Pickup Type", &mPickupType);
		ImGui::InputFloat("Buff Value", &mBuffValue);
		ImGui::InputFloat("Effect Time", &mEffectTime);
	}
}