#include <hollowpch.h>
#include "Script.h"

namespace Hollow {

	Script Script::instance;

	void Script::Init()
	{
		mScriptPath = "";
	}

	void Script::Clear()
	{
	}


	void Script::DebugDisplay()
	{
		char* str = (char*)mScriptPath.c_str();
		ImGui::InputText("Path", str, 50);
		mScriptPath = str;
	}

	void Script::Serialize(rapidjson::Value::Object data)
	{
		if (data.HasMember("FilePath"))
		{
			mScriptPath = data["FilePath"].GetString();
		}
	}

	void Script::DeSerialize(rapidjson::Writer<rapidjson::StringBuffer>& writer)
	{
		JSONHelper::Write("FilePath", mScriptPath, writer);
	}
}
