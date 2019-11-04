#include <hollowpch.h>
#include "Script.h"

namespace Hollow {

	Script Script::instance;

	void Script::Init()
	{
		scriptPath = "";
	}

	void Script::Clear()
	{
		Init();
	}


	void Script::DebugDisplay()
	{
		if (ImGui::TreeNode("Script"))
		{
			char* str = (char*)scriptPath.c_str();
			ImGui::InputText("Path", str, 50);
			scriptPath = str;
			ImGui::TreePop();
		}
	}

	void Script::Serialize(rapidjson::Value::Object data)
	{
		if (data.HasMember("FilePath"))
		{
			scriptPath = data["FilePath"].GetString();
		}
	}

}