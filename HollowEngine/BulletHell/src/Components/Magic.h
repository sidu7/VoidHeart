#pragma once
#include "Hollow.h"
#include "Hollow/Components/Component.h"

namespace BulletHell
{
	class Magic : public Hollow::Component
	{
		REGISTERCOMPONENT(Magic);
	public:
		
		/// <summary>
		/// Initializes the Attack component.
		/// </summary>
		void Init();

		/// <summary>
		/// Serializes the specified data of the Attack component from the JSON file.
		/// </summary>
		/// <param name="data">JSON object.</param>
		void Serialize(rapidjson::Value::Object data);

		/// <summary>
		/// DeSerialize the current Attack component into a JSON file.
		/// </summary>
		/// <param name="writer">The writer of the JSON file.</param>
		void DeSerialize(rapidjson::Writer<rapidjson::StringBuffer>& writer);

		/// <summary>
		/// Clears the Magic component.
		/// </summary>
		void Clear();

		/// <summary>
		/// To set the Debug Display for the Attack Component.
		/// </summary>
		void DebugDisplay();

	private:
		struct Spell {
			std::string mName;
			std::string mScriptPath;
		};

	public:
		std::string mLeftHandScriptPath;
		std::string mRightHandScriptPath;
		std::string mCombineHandScriptPath;

		std::vector<Spell*> mSpells;

	};
}