#include "Magic.h"

#include "Hollow/Common.h"

namespace BulletHell
{
	Magic Magic::instance;

	void Magic::Init()
	{
		// Set left and right hand spells to empty
		SpellData* pEmptySpell = new SpellData{ "EMPTY", "Resources/Scripts/Spells/Sp_Empty.lua", 0, 0.0f, 0.1f, "Resources/Textures/star.png", 1.0f};
		mSpells.push_back(pEmptySpell);
		mLeftHandSpell = pEmptySpell;
		mRightHandSpell = pEmptySpell;
	}

	void Magic::Serialize(rapidjson::Value::Object data)
	{
		if (data.HasMember("LeftHandScript"))
		{
			mLeftHandScriptPath = data["LeftHandScript"].GetString();
		}
		if (data.HasMember("RightHandScript"))
		{
			mRightHandScriptPath = data["RightHandScript"].GetString();
		}
		if (data.HasMember("CombineHandScript"))
		{
			mCombineHandScriptPath = data["CombineHandScript"].GetString();
		}
	}

	void Magic::DeSerialize(rapidjson::Writer<rapidjson::StringBuffer>& writer)
	{
		Hollow::JSONHelper::Write<std::string>("LeftHandScript", mLeftHandScriptPath, writer);
		Hollow::JSONHelper::Write<std::string>("RightHandScript", mRightHandScriptPath, writer);
		Hollow::JSONHelper::Write<std::string>("CombineHandScript", mCombineHandScriptPath, writer);
	}

	void Magic::Clear()
	{
		
	}

	void Magic::DebugDisplay()
	{
		Hollow::ImGuiHelper::InputText("Left Hand Path", mLeftHandScriptPath);
		Hollow::ImGuiHelper::InputText("Right Hand Path", mRightHandScriptPath);
		Hollow::ImGuiHelper::InputText("Combine Hand Path", mCombineHandScriptPath);

		ImGui::Text("Right Hand Spell %s", mRightHandSpell->mName.c_str());
		ImGui::Text("Left Hand Spell %s", mLeftHandSpell->mName.c_str());

		//ImGui::Columns(2, 0, true);
		//ImGui::Separator();
		//ImGui::Text("ID"); ImGui::NextColumn();
		for (auto& spell : mSpells)
		{
			ImGui::Text("%s", spell->mName);
			ImGui::Text("RH CD: %f", spell->mRightHandCooldown); ImGui::SameLine();
			ImGui::Text("LH CD: %f", spell->mLeftHandCooldown);
		}
	}
	
}
