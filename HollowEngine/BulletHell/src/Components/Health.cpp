#include "Health.h"

namespace BulletHell
{
	Health Health::instance;

	void Health::Init()
	{
	}

	void Health::Serialize(rapidjson::Value::Object data)
	{
		if (data.HasMember("HitPoints"))
		{
			mHitPoints = data["HitPoints"].GetInt();
		}
		if (data.HasMember("IsAlive"))
		{
			mIsAlive = data["IsAlive"].GetBool();
		}
		if (data.HasMember("Invincible"))
		{
			mInvincible = data["Invincible"].GetBool();
		}
		if (data.HasMember("InvincibleTime"))
		{
			mInvincibleTime = data["InvincibleTime"].GetFloat();
		}
	}

	void Health::DeSerialize(rapidjson::Writer<rapidjson::StringBuffer>& writer)
	{
		Hollow::JSONHelper::Write<int>("HitPoints", mHitPoints, writer);
		Hollow::JSONHelper::Write<bool>("IsAlive", mIsAlive, writer);
		Hollow::JSONHelper::Write<bool>("Invincible", mInvincible, writer);
		Hollow::JSONHelper::Write<float>("InvincibileTime", mInvincibleTime, writer);
	}

	void Health::Clear()
	{
	}

	void Health::DebugDisplay()
	{
		ImGui::InputInt("Hit Points", &mHitPoints);
		ImGui::Checkbox("Is Alive", &mIsAlive);
		ImGui::Checkbox("Invincible", &mInvincible);
		ImGui::InputFloat("Invincible Time", &mInvincibleTime);
		ImGui::InputFloat("Current Invincible Timer", &mCurrentInvincibleTime);
	}

}