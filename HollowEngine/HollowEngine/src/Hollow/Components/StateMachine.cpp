#include <hollowpch.h>
#include "StateMachine.h"

#include "Hollow/Managers/ResourceManager.h"

namespace Hollow
{
	StateMachine StateMachine::instance;

	void StateMachine::Init()
	{
	}

	void StateMachine::Clear()
	{
	}

	void StateMachine::Serialize(rapidjson::Value::Object data)
	{
		if (data.HasMember("StateFile"))
		{
			mStates = ResourceManager::Instance().ReadStateMachineFile(data["StateFile"].GetString());
		}
	}

	void StateMachine::DebugDisplay()
	{
	}
}