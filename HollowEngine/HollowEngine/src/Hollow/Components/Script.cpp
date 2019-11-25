#include <hollowpch.h>
#include "Script.h"
#include "Hollow/Utils/ImGuiHelper.h"

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
		ImGuiHelper::InputText("Script File", mScriptPath);
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
