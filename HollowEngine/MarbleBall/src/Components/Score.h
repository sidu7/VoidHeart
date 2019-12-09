#pragma once
#include "Hollow/Components/Component.h"

namespace MarbleBall
{
	class Score : public Hollow::Component
	{
		REGISTERCOMPONENT(Score);
	public:
		/// <summary>
		/// Destructor for the instance of the <see cref="Score"/> class.
		/// </summary>
		~Score() override {}

		/// <summary>
		/// Initializes the Scor component.
		/// </summary>
		void Init() override;

		/// <summary>
		/// Serializes the specified data of the Score component from the JSON file.
		/// </summary>
		/// <param name="data">JSON object.</param>
		void Serialize(rapidjson::Value::Object data) override;

		/// <summary>
		/// DeSerialize the current Score component into a JSON file.
		/// </summary>
		/// <param name="writer">The writer of the JSON file.</param>
		void DeSerialize(rapidjson::Writer<rapidjson::StringBuffer>& writer) override;

		/// <summary>
		/// Clears the Score component.
		/// </summary>
		void Clear() override;

		/// <summary>
		/// To set the Debug Display for the Scor Component.
		/// </summary>
		void DebugDisplay() override;

	public:
		int mScore;
		int mScoreIncrement;
		bool mGoalReached;
	};
}