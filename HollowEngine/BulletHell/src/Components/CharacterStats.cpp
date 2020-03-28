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
			sol::constructors<CharacterStats()>(),
			"movementSpeed", &CharacterStats::mMovementSpeed,
			"movementSpeedFactor", &CharacterStats::mMovementSpeedFactor,
			"dashSpeed", &CharacterStats::mDashSpeed,
			"dashSpeedFactor", &CharacterStats::mDashSpeedFactor,
			"damageFactor", &CharacterStats::mDamageFactor);

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
		if (data.HasMember("MovementSpeedFactor"))
		{
			mMovementSpeedFactor = data["MovementSpeedFactor"].GetFloat();
		}
		if (data.HasMember("DamageFactor"))
		{
			mDamageFactor = data["DamageFactor"].GetFloat();
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
		ImGui::InputFloat("Dash Speed Factor", &mDashSpeed);
		ImGui::InputFloat("Movement Speed", &mMovementSpeed);
		ImGui::InputFloat("Movement Speed Factor", &mMovementSpeedFactor);
		ImGui::InputFloat("Damage", &mDamageFactor);
		ImGui::InputFloat("Fire Rate", &mFireRate);
	}
}
