#include "Magic.h"

// TODO: Find a better place to put SpellType enum
// Coupling components this closely could cause issues
#include "Spell.h"

namespace BulletHell
{
	Magic Magic::instance;
	// Create all combined spells for storage in the map
	// Map is [int, Spell*] where int is the bit mask for the spell types
	std::unordered_map<int, Magic::SpellData*> Magic::mCombinedSpells = Magic::CreateSpellMap();

	void Magic::Init()
	{	
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

		if (mRightHandSpell != nullptr)
		{
			ImGui::Text("Right Hand Spell %s", mRightHandSpell->mName.c_str());
		}
		if (mLeftHandSpell != nullptr)
		{
			ImGui::Text("Left Hand Spell %s", mLeftHandSpell->mName.c_str());
		}
		if (mCombinedSpell != nullptr)
		{
			ImGui::Text("Combined Spell %s", mCombinedSpell->mName.c_str());
		}

		//ImGui::Columns(2, 0, true);
		//ImGui::Separator();
		//ImGui::Text("ID"); ImGui::NextColumn();
		for (auto& spell : mSpells)
		{
			ImGui::Text("%s", spell->mName);
			ImGui::Text("RH CD: %f", spell->mRightHandCooldown); ImGui::SameLine();
			ImGui::Text("LH CD: %f", spell->mLeftHandCooldown);
		}

		ImGui::Text("Combined Spells");
		for (auto& spell : mCombinedSpells)
		{
			ImGui::Text("%s:", spell.second->mName); ImGui::SameLine();
			ImGui::Text("%f", spell.second->mCombinedCooldown);
		}
	}

	std::unordered_map<int, Magic::SpellData*> Magic::CreateSpellMap()
	{
		std::unordered_map<int, Magic::SpellData*> spellMap;
		Magic::SpellData* pCombinedSpell = new Magic::SpellData{ "Fire + Fire", "Spells/FireFire", COMBINED,  0.0f, 0.0f, "", 3.0f, "Resources/Textures/FireFire.png" };
		spellMap.emplace(SpellType::FIRE | SpellType::FIRE, pCombinedSpell);

		pCombinedSpell = new Magic::SpellData{ "Air + Air", "Spells/AirAir", COMBINED,  0.0f, 0.0f, "", 1.0f, "Resources/Textures/AirAir.png" };
		spellMap.emplace(SpellType::AIR | SpellType::AIR, pCombinedSpell);

		pCombinedSpell = new Magic::SpellData{ "Earth + Earth", "Spells/EarthEarth", COMBINED, 0.0f, 0.0f, "", 3.0f, "Resources/Textures/EarthEarth.png" };
		spellMap.emplace(SpellType::EARTH | SpellType::EARTH, pCombinedSpell);

		pCombinedSpell = new Magic::SpellData{ "Water + Water", "Spells/Empty", COMBINED, 0.0f, 0.0f, "", 1.0f, "Resources/Textures/WaterWater.png" };
		spellMap.emplace(SpellType::WATER | SpellType::WATER, pCombinedSpell);

		pCombinedSpell = new Magic::SpellData{ "Fire + Air", "Spells/FireAir", COMBINED,  0.0f, 0.0f, "", 0.1f, "Resources/Textures/FireAir.png" };
		spellMap.emplace(SpellType::FIRE | SpellType::AIR, pCombinedSpell);

		pCombinedSpell = new Magic::SpellData{ "Fire + Earth", "Spells/Empty", COMBINED, 0.0f, 0.0f, "", 1.0f, "Resources/Textures/FireEarth.png" };
		spellMap.emplace(SpellType::FIRE | SpellType::EARTH, pCombinedSpell);

		pCombinedSpell = new Magic::SpellData{ "Fire + Water", "Spells/Empty", COMBINED, 0.0f, 0.0f, "", 1.0f, "Resources/Textures/FireWater.png" };
		spellMap.emplace(SpellType::FIRE | SpellType::WATER, pCombinedSpell);

		pCombinedSpell = new Magic::SpellData{ "Air + Earth", "Spells/Empty", COMBINED, 0.0f, 0.0f, "", 1.0f, "Resources/Textures/AirEarth.png" };
		spellMap.emplace(SpellType::AIR | SpellType::EARTH, pCombinedSpell);

		pCombinedSpell = new Magic::SpellData{ "Air + Water", "Spells/Empty", COMBINED, 0.0f, 0.0f, "", 1.0f, "Resources/Textures/AirWater.png" };
		spellMap.emplace(SpellType::AIR | SpellType::WATER, pCombinedSpell);

		pCombinedSpell = new Magic::SpellData{ "Earth + Water", "Spells/Empty", COMBINED, 0.0f, 0.0f, "", 1.0f, "Resources/Textures/EarthWater.png" };
		spellMap.emplace(SpellType::EARTH | SpellType::WATER, pCombinedSpell);

		return spellMap;
	}
	
}
