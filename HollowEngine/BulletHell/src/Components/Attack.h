#pragma once
#include "Hollow.h"
#include "Hollow/Components/Component.h"

namespace BulletHell
{
	class Attack : public Hollow::Component
	{
		REGISTERCOMPONENT(Attack);
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
		/// Clears the Attack component.
		/// </summary>
		void Clear();

		/// <summary>
		/// To set the Debug Display for the Attack Component.
		/// </summary>
		void DebugDisplay();

	public:
        Hollow::GameObject* mpTarget;
		std::string mScriptPath;
		float mBaseAttackTime;
		float mCurrentAttackTime;
		bool mFireOnce;
		bool mIsFired;
		bool mShouldAttack;

		bool mIsActive;
		float mChargeTime;
	};
}