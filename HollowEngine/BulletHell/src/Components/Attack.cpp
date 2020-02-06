#include "Attack.h"

#include "Hollow/Managers/ScriptingManager.h"

#include "Hollow/Components/Transform.h"

#include "Hollow/Managers/GameObjectManager.h"

namespace BulletHell
{
	Attack Attack::instance;

	void Attack::Init()
	{
		mIsFired = false;
	}

	void Attack::Serialize(rapidjson::Value::Object data)
	{
		if (data.HasMember("Script"))
		{
			mScriptPath = data["Script"].GetString();
		}
		if (data.HasMember("BaseAttackTime"))
		{
			mBaseAttackTime = data["BaseAttackTime"].GetFloat();
		}
		if (data.HasMember("FireOnce"))
		{
			mFireOnce = data["FireOnce"].GetBool();
		}
        if (data.HasMember("Target"))
        {
            int targetType = data["Target"].GetInt();
            std::vector<Hollow::GameObject*> possibleTarget = Hollow::GameObjectManager::Instance().GetObjectByType(targetType);
            if (possibleTarget.empty())
            {
                mpTarget = nullptr;
            }
            else
            {
                mpTarget = possibleTarget[0];
            }
        }
	}

	void Attack::DeSerialize(rapidjson::Writer<rapidjson::StringBuffer>& writer)
	{
		Hollow::JSONHelper::Write<std::string>("Script", mScriptPath, writer);
		Hollow::JSONHelper::Write<float>("BaseAttackTime", mBaseAttackTime, writer);
		Hollow::JSONHelper::Write<bool>("FireOnce", mFireOnce, writer);
	}

	void Attack::Clear()
	{
	}

	void Attack::DebugDisplay()
	{
		char* str = (char*)mScriptPath.c_str();
		ImGui::InputText("Path", str, 50);
		mScriptPath = str;

		ImGui::InputFloat("Base Attack Time", &mBaseAttackTime);
		ImGui::Text("Current Attack Time %f", mCurrentAttackTime);

		// Attack testing button
		if (ImGui::Button("Fire Attack"))
		{
			auto& lua = Hollow::ScriptingManager::Instance().lua;
			lua["attackPosition"] = mpOwner->GetComponent<Hollow::Transform>()->mPosition;
			lua.script_file(mScriptPath);
		}
	}
} 