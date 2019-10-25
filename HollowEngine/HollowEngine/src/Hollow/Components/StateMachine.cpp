#include <hollowpch.h>
#include "StateMachine.h"

#include "Hollow/Managers/ResourceManager.h"

#include "Hollow/Core/Data/StateData.h"

namespace Hollow
{
	StateMachine StateMachine::instance;

	void StateMachine::Init()
	{
		mCurrentState = nullptr;
		mPreviousState = nullptr;
	}

	void StateMachine::Clear()
	{
		mCurrentState = nullptr;
		mPreviousState = nullptr;
		mStates.clear();
	}

	void StateMachine::Serialize(rapidjson::Value::Object data)
	{
		if (data.HasMember("StateFile"))
		{
			mStates = ResourceManager::Instance().ReadStateMachineFile(data["StateFile"].GetString());
		}
		if (data.HasMember("StartState"))
		{
			mCurrentState = mStates[State::FindState(mStates, data["StartState"].GetString())];
			mPreviousState = mCurrentState;
		}
	}

	void StateMachine::DebugDisplay()
	{
	}
}