#pragma once
#include "Component.h"

namespace Hollow
{
	struct State;

	class StateMachine : public Component
	{
		REGISTERCOMPONENT(StateMachine);
	public:
		HOLLOW_API void Init() override;
		HOLLOW_API void Clear() override;
		HOLLOW_API void Serialize(rapidjson::Value::Object data) override;
		HOLLOW_API void DeSerialize(rapidjson::Writer<rapidjson::StringBuffer>& writer) override;
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