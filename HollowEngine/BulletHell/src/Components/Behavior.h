#pragma once
#include "Hollow/Components/Component.h"

namespace BulletHell
{
	class Behavior : public Hollow::Component
	{
		REGISTERCOMPONENT(Behavior);
	public:
		/// <summary>
		/// Initializes the Behavior component.
		/// </summary>
		void Init();

		/// <summary>
		/// Serializes the specified data of the Behavior component from the JSON file.
		/// </summary>
		/// <param name="data">JSON object.</param>
		void Serialize(rapidjson::Value::Object data);

		/// <summary>
		/// DeSerialize the current Behavior component into a JSON file.
		/// </summary>
		/// <param name="writer">The writer of the JSON file.</param>
		void DeSerialize(rapidjson::Writer<rapidjson::StringBuffer>& writer);

		/// <summary>
		/// Clears the Behavior component.
		/// </summary>
		void Clear();

		/// <summary>
		/// To set the Debug Display for the Behavior Component.
		/// </summary>
		void DebugDisplay();

	public:
		std::string mCurrentBehavior;
		std::unordered_map<std::string, std::string> mBehaviorStates;
		std::vector<std::pair<int, std::string>> mThreshold;
		float mTransitionTime;
	};
}