#include <hollowpch.h>
#include "Script.h"
#include "Hollow/Utils/ImGuiHelper.h"

namespace Hollow {

	Script Script::instance;

	void Script::Init()
	{
		mIsActive = true;
	}

	void Script::Clear()
	{
	}


	void Script::DebugDisplay()
	{
		ImGui::InputInt("Count", &mScriptCount);
		for (int i = 0; i < mScriptCount; ++i)
		{
			ImGuiHelper::InputText("File: ", mScripts[i]);
		}
	}

	void Script::Serialize(rapidjson::Value::Object data)
	{
		if(data.HasMember("Active"))
		{
			mIsActive = data["Active"].GetBool();
		}
		if (data.HasMember("Scripts"))
		{
			auto scripts = data["Scripts"].GetArray();
			for (int i = 0; i < scripts.Size(); ++i)
			{
				mScripts.push_back(scripts[i].GetString());
			}
			mScriptCount = mScripts.size();
		}
	}

	void Script::DeSerialize(rapidjson::Writer<rapidjson::StringBuffer>& writer)
	{
		writer.Key("Active");
		writer.Bool(mIsActive);
		writer.Key("Scripts");
		writer.StartArray();
		// generalize the array
		for (auto s : mScripts)
		{
			writer.String(s.c_str());
		}
		writer.EndArray();
	}
}
