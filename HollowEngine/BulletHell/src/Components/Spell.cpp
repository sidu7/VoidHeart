#include "Spell.h"

namespace BulletHell
{
	Spell Spell::instance;

	void Spell::Init()
	{
	}

	void Spell::Serialize(rapidjson::Value::Object data)
	{
		if (data.HasMember("Name"))
		{
			mName = data["Name"].GetString();
		}
		if (data.HasMember("Script"))
		{
			mScriptPath = data["Script"].GetString();
		}
	}

	void Spell::DeSerialize(rapidjson::Writer<rapidjson::StringBuffer>& writer)
	{
		Hollow::JSONHelper::Write<std::string>("Name", mName, writer);
		Hollow::JSONHelper::Write<std::string>("Script", mScriptPath, writer);
	}

	void Spell::Clear()
	{
	}

	void Spell::DebugDisplay()
	{
		Hollow::ImGuiHelper::InputText("Name", mName);
		Hollow::ImGuiHelper::InputText("Script Path", mScriptPath);
	}
}