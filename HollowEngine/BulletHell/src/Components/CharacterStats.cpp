#include "CharacterStats.h"
#include "Hollow/Managers/ScriptingManager.h"

namespace BulletHell
{
	CharacterStats CharacterStats::instance;

	void BulletHell::CharacterStats::Init()
	{
		mDashSpeedFactor = 1.0f;
		mFireRate = 1.0f; // this is a factor
		mMovementSpeedFactor = 1.0f; // this should be absolute value

		// Register to Lua
		auto& lua = Hollow::ScriptingManager::Instance().lua;
		lua.new_usertype<CharacterStats>("Stats",
			sol::constructors<CharacterStats()>());
		Hollow::ScriptingManager::Instance().mGameObjectType["GetStats"] = &Hollow::GameObject::GetComponent<CharacterStats>;
	}

	void CharacterStats::Serialize(rapidjson::Value::Object data)
	{
		if (data.HasMember("DashSpeed"))
		{
			mDashSpeed = data["DashSpeed"].GetFloat();
		}
		if (data.HasMember("RateOfFire"))
		{
			mFireRate = data["RateOfFire"].GetFloat();
		}
		if (data.HasMember("MovementSpeed"))
		{
			mMovementSpeed = data["MovementSpeed"].GetFloat();
		}
		if (data.HasMember("Damage"))
		{
			mDamageFactor = data["Damage"].GetFloat();
		}
	}

	void CharacterStats::DeSerialize(rapidjson::Writer<rapidjson::StringBuffer>& writer)
	{
		Hollow::JSONHelper::Write<float>("DashSpeed", mDashSpeed, writer);
		Hollow::JSONHelper::Write<float>("MovementSpeed", mMovementSpeed, writer);
		Hollow::JSONHelper::Write<float>("Damage", mDamageFactor, writer);
		Hollow::JSONHelper::Write<float>("RateOfFire", mFireRate, writer);
	}

	void CharacterStats::Clear()
	{
	}

	void CharacterStats::DebugDisplay()
	{
		ImGui::InputFloat("Dash Speed", &mDashSpeed);
		ImGui::InputFloat("Movement Speed", &mMovementSpeed);
		ImGui::InputFloat("Damage", &mDamageFactor);
		ImGui::InputFloat("Fire Rate", &mFireRate);
	}
}
