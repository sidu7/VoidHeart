#pragma once
#include "Component.h"

namespace Hollow
{
	struct State;

	class StateMachine : public Component
	{
		REGISTERCOMPONENT(StateMachine);
	public:
		/// <summary>
		/// Initializes the StateMachine component.
		/// </summary>
		HOLLOW_API void Init() override;

		/// <summary>
		/// Clears the StateMachine component.
		/// </summary>
		HOLLOW_API void Clear() override;

		/// <summary>
		/// Serializes the specified data of the StateMachine component from the JSON file.
		/// </summary>
		/// <param name="data">JSON object.</param>
		HOLLOW_API void Serialize(rapidjson::Value::Object data) override;

		/// <summary>
		/// DeSerialize the current StateMachine component into a JSON file.
		/// </summary>
		/// <param name="writer">The writer of the JSON file.</param>
		HOLLOW_API void DeSerialize(rapidjson::Writer<rapidjson::StringBuffer>& writer) override;

		/// <summary>
		/// To set the Debug Display for the StateMachine Component.
		/// </summary>
		HOLLOW_API void DebugDisplay() override;

	private:
		std::string mStateFilePath;
		std::string mInitState;
		
	public:
		std::vector<State*> mStates;
		State* mCurrentState;
		State* mPreviousState;
		bool mNeedChangeState;
	};
}