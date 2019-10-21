#pragma once
#include "Component.h"

namespace Hollow
{
	struct State
	{
		std::string mName;
		unsigned int mIndex;
		std::vector<unsigned int> mNextStates;
	};

	class HOLLOW_API StateMachine : public Component
	{
		REGISTERCOMPONENT(StateMachine);
	public:
		void Init();
		void Clear();
		void Serialize(rapidjson::Value::Object data);
		void DebugDisplay();

	public:
		std::vector<State> mStates;
		// mCurrentState
		// mPrevState
	};
}