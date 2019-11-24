#include "Attack.h"

#include "Hollow/Managers/ScriptingManager.h"

namespace BulletHell
{
	Attack Attack::instance;

	void Attack::Init()
	{
	}
	void Attack::Serialize(rapidjson::Value::Object data)
	{
		if (data.HasMember("Script"))
		{
			mScriptPath = data["Script"].GetString();
		}
	}
	void Attack::DeSerialize(rapidjson::Writer<rapidjson::StringBuffer>& writer)
	{
		Hollow::JSONHelper::Write<std::string>("Script", mScriptPath, writer);
	}
	void Attack::Clear()
	{
	}
	void Attack::DebugDisplay()
	{
		if (ImGui::TreeNode("Attack"))
		{
			char* str = (char*)mScriptPath.c_str();
			ImGui::InputText("Path", str, 50);
			mScriptPath = str;

			// TODO: REMOVE THIS, for testing only
			if (ImGui::Button("Fire Attack"))
			{
				auto& lua = Hollow::ScriptingManager::Instance().lua;
				lua.script_file(mScriptPath);
			}

			ImGui::TreePop();
		}
	}
}