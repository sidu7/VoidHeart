#pragma once
#include "Hollow.h"
#include "Hollow/Components/Component.h"

namespace BulletHell
{
	// Bit field for spell type
	enum SpellType
	{
		EMPTY = 0,
		FIRE = 1,
		AIR = 2,
		EARTH = 4,
		WATER = 8,
		COMBINED = 16
	};

	class Spell : public Hollow::Component
	{
		REGISTERCOMPONENT(Spell);
	public:
		void Init();
		void Serialize(rapidjson::Value::Object data);
		void DeSerialize(rapidjson::Writer<rapidjson::StringBuffer>& writer);
		void Clear();
		void DebugDisplay();

	public:
		std::string mName;
		std::string mScriptPath;
		int mSpellType;
		float mUIRotation;
		float mParticleSize;
		std::string mParticleTexturePath;
		float mCooldown;

	};
}