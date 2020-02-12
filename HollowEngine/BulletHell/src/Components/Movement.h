#pragma once
#include "Hollow/Components/Component.h"

namespace BulletHell
{
	class Movement : public Hollow::Component
	{
		REGISTERCOMPONENT(Movement);
	public:
		/// <summary>
		/// Initializes the Movement component.
		/// </summary>
		void Init();

		/// <summary>
		/// Serializes the specified data of the Movement component from the JSON file.
		/// </summary>
		/// <param name="data">JSON object.</param>
		void Serialize(rapidjson::Value::Object data);

		/// <summary>
		/// DeSerialize the current Movement component into a JSON file.
		/// </summary>
		/// <param name="writer">The writer of the JSON file.</param>
		void DeSerialize(rapidjson::Writer<rapidjson::StringBuffer>& writer);

		/// <summary>
		/// Clears the Movement component.
		/// </summary>
		void Clear();

		/// <summary>
		/// To set the Debug Display for the Movement Component.
		/// </summary>
		void DebugDisplay();

	public:
		std::string mScriptPath;
		float mMoveDebuffFactor;
	};
}