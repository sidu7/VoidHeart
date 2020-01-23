#include <hollowpch.h>
#include "StateMachineSystem.h"

#include "Hollow/Components/StateMachine.h"

#include "Hollow/Managers/InputManager.h"

#include "Hollow/Core/Data/StateData.h"

namespace Hollow
{
	StateMachineSystem StateMachineSystem::instance;

	StateMachineSystem::~StateMachineSystem()
	{
	}

	void StateMachineSystem::AddGameObject(GameObject* object)
	{
		CheckAllComponents<StateMachine>(object);
	}

	void StateMachineSystem::Update()
	{
		for (unsigned int i = 0; i < mGameObjects.size(); ++i)
		{
			StateMachine* state = mGameObjects[i]->GetComponent<StateMachine>();

			if (state->mNeedChangeState)
			{
				State* temp = state->mCurrentState;
				state->mCurrentState = state->mPreviousState;
				state->mPreviousState = temp;
				state->mNeedChangeState = false;
				continue;
			}

			// Check for state change by inputs
			std::vector<std::string>& inputs = state->mCurrentState->mInputs;
			std::vector<State::StateInputCondition>& inputconditions = state->mCurrentState->mInputConditions;
			for (unsigned int i = 0; i < inputs.size(); ++i)
			{	
				bool stateChanged = false;
				switch (inputconditions[i])
				{
				case State::StateInputCondition::KEY_RELEASED:
					if (InputManager::Instance().IsKeyReleased(inputs[i]))
					{
						stateChanged = true;
					}
					break;
				case State::StateInputCondition::KEY_PRESSED:
					if (InputManager::Instance().IsKeyPressed(inputs[i]))
					{
						stateChanged = true;
					}
					break;
				case State::StateInputCondition::KEY_TRIGGERED:
					if (InputManager::Instance().IsKeyTriggered(inputs[i]))
					{
						stateChanged = true;
					}
					break;
				}
				if (stateChanged)
				{
					state->mPreviousState = state->mCurrentState;
					state->mCurrentState = state->mStates[state->mCurrentState->mInputStates[i]];
					//HW_CORE_TRACE("State changed from {0} to {1}", state->mPreviousState->mName, state->mCurrentState->mName);
					break;
				}
			}
		}
	}
}