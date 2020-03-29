#pragma once
#include "Hollow.h"
#include "Hollow/Components/Component.h"

namespace BulletHell
{
	class Health : public Hollow::Component
	{
		REGISTERCOMPONENT(Health);
	public:
		/// <summary>
		/// Initializes the Health component.
		/// </summary>
		void Init();

		/// <summary>
		/// Serializes the specified data of the Health component from the JSON file.
		/// </summary>
		/// <param name="data">JSON object.</param>
		void Serialize(rapidjson::Value::Object data);

		/// <summary>
		/// DeSerialize the current Health component into a JSON file.
		/// </summary>
		/// <param name="writer">The writer of the JSON file.</param>
		void DeSerialize(rapidjson::Writer<rapidjson::StringBuffer>& writer);

		/// <summary>
		/// Clears the Health component.
		/// </summary>
		void Clear();

		/// <summary>
		/// To set the Debug Display for the Health Component.
		/// </summary>
		void DebugDisplay();

	public:
		int mHitPoints;
		bool mIsAlive;
		bool mInvincible;
        bool mIsHit; // sending to lua event, if set to true lua script will react and set it to false
		float mInvincibleTime;
		float mCurrentInvincibleTime;
        float mHitReactionTime;
        float mCurrentHitReactionTime;
	};
}