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
		mNeedChangeState = false;
		mStateFilePath = "";
		mInitState = "";
	}

	void StateMachine::Clear()
	{
		mStates.clear();
	}

	void StateMachine::Serialize(rapidjson::Value::Object data)
	{
		if (data.HasMember("StateFile"))
		{
			mStateFilePath = data["StateFile"].GetString();
			mStates = ResourceManager::Instance().ReadStateMachineFile(mStateFilePath);
		}
		if (data.HasMember("StartState"))
		{
			mInitState = data["StartState"].GetString();
			mCurrentState = mStates[State::FindState(mStates, mInitState)];
			mPreviousState = mCurrentState;
		}
	}

	void StateMachine::DeSerialize(rapidjson::Writer<rapidjson::StringBuffer>& writer)
	{
		JSONHelper::Write("StateFile", mStateFilePath, writer);
		JSONHelper::Write("StartState", mInitState, writer);
	}

	void StateMachine::DebugDisplay()
	{
	}
}