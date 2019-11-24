#include "Health.h"

//namespace BulletHell
//{
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
	}

	void Health::DeSerialize(rapidjson::Writer<rapidjson::StringBuffer>& writer)
	{
		Hollow::JSONHelper::Write<int>("HitPoints", mHitPoints, writer);
		Hollow::JSONHelper::Write<bool>("IsAlive", mIsAlive, writer);
	}

	void Health::Clear()
	{
	}

	void Health::DebugDisplay()
	{
		//if (ImGui::TreeNode("Health"))
		{
			//ImGui::InputInt("Hit Points", &mHitPoints);
			//ImGui::Checkbox("Is Alive", &mIsAlive);
		}
	}

//}