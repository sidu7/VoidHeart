#pragma once
#include <vector>
#include <string>

namespace Hollow
{
	struct State
	{
		enum StateInputCondition
		{
			KEY_RELEASED,
			KEY_PRESSED,
			KEY_TRIGGERED
		};

		std::string mName;
		unsigned int mIndex;
		bool mIsLoop;
		std::vector<std::string> mEvents;
		std::vector<unsigned int> mEventStates;
		std::vector<unsigned int> mInputs;
		std::vector<unsigned int> mInputStates;
		std::vector<StateInputCondition> mInputConditions;

		inline static unsigned int FindState(std::vector<State*>& states, std::string name)
		{
			for (unsigned int i = 0; i < states.size(); ++i)
			{
				if (states[i]->mName == name)
				{
					return i;
				}
			}
			return -1;
		}
	};
}