#pragma once
#include "Hollow.h"
#include "Hollow/Components/Component.h"

namespace BulletHell
{
	class Magic : public Hollow::Component
	{
		REGISTERCOMPONENT(Magic);
	public:
		void Init();
		void Serialize(rapidjson::Value::Object data);
		void DeSerialize(rapidjson::Writer<rapidjson::StringBuffer>& writer);
		void Clear();
		void DebugDisplay();

	public:
		struct SpellData {
			std::string mName;
			std::string mScriptPath;
			int mSpellType;
			float mUIRotation;
			float mParticleSize;
			std::string mParticleTexturePath;
			float mCooldown;
			std::string mUITexturePath;

			float mRightHandCooldown;
			float mLeftHandCooldown;
			float mCombinedCooldown;
			float mCooldownModifier = 1.0f;
		};

	public:
		std::string mLeftHandScriptPath;
		std::string mRightHandScriptPath;
		std::string mCombineHandScriptPath;

		SpellData* mLeftHandSpell;
		SpellData* mRightHandSpell;
		SpellData* mCombinedSpell;

		std::vector<SpellData*> mSpells;
		static std::unordered_map<int, SpellData*> mCombinedSpells;

	private:
		static std::unordered_map<int, SpellData*> CreateSpellMap();

	};
}