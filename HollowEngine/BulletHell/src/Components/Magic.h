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
			glm::vec3 mLightColor; 
			std::string mUITexturePath;

			float mRightHandCooldown = 0.0f;
			float mLeftHandCooldown = 0.0f;
			float mCombinedCooldown = 0.0f;
			float mCooldownModifier = 1.0f;

			bool mCollected = false;
		};

	public:
		std::string mLeftHandScriptPath;
		std::string mRightHandScriptPath;
		std::string mCombineHandScriptPath;

		SpellData* mLeftHandSpell;
		SpellData* mRightHandSpell;
		SpellData* mCombinedSpell;

		static std::unordered_map<int, SpellData*> mBasicSpells;
		static std::unordered_map<int, SpellData*> mCombinedSpells;

	private:
		static std::unordered_map<int, SpellData*> CreateBasicSpellMap();
		static std::unordered_map<int, SpellData*> CreateCombinedSpellMap();

	};
}