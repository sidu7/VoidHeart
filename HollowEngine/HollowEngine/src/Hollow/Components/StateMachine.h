#pragma once
#include "Component.h"

namespace Hollow
{
	struct State;

	class StateMachine : public Component
	{
		REGISTERCOMPONENT(StateMachine);
	public:
		HOLLOW_API void Init();
		HOLLOW_API void Clear();
		HOLLOW_API void Serialize(rapidjson::Value::Object data);
		HOLLOW_API void DebugDisplay();

	public:
		std::vector<State*> mStates;
		State* mCurrentState;
		State* mPreviousState;
		bool mNeedChangeState;
	};
}