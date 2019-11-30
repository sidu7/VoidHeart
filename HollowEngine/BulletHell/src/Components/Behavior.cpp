#include <Hollow.h>
#include "Behavior.h"

namespace BulletHell
{
	Behavior Behavior::instance;

	void Behavior::Init()
	{
	}

	void Behavior::Serialize(rapidjson::Value::Object data)
	{
		// Make pairs of state name to lua file path
		if (data.HasMember("BehaviorStates"))
		{
			for (auto& states : data["BehaviorStates"].GetArray())
			{
				auto pair = states.GetArray();
				std::string name = pair[0].GetString();
				mBehaviorStates[name] = pair[1].GetString();
			}
		}

		// Make pairs of hp threshold to state strings
		if (data.HasMember("Threshold"))
		{
			for (auto& threshold : data["Threshold"].GetArray())
			{
				auto pair = threshold.GetArray();
				int hpThreshold = pair[0].GetInt();
				std::string stateString = pair[1].GetString();
				mThreshold.push_back(std::make_pair(hpThreshold, stateString));
			}
		}
	}

	void Behavior::DeSerialize(rapidjson::Writer<rapidjson::StringBuffer>& writer)
	{
		writer.Key("BehaviorStates");
		writer.StartArray();
		for (auto state : mBehaviorStates)
		{
			writer.StartArray();
			writer.String(state.first.c_str());
			writer.String(state.second.c_str());
			writer.EndArray();
		}
		writer.EndArray();
	}

	void Behavior::Clear()
	{
	}

	void Behavior::DebugDisplay()
	{
		Hollow::ImGuiHelper::InputText("Current Behavior", mCurrentBehavior);

		ImGui::Text("Lua File ----- Behavior Name");
		for (auto& state : mBehaviorStates)
		{
			Hollow::ImGuiHelper::InputText(state.first.c_str(), state.second);
			ImGui::Separator();
		}

	}
}