#pragma once
#include <vector>
#include <string>

namespace Hollow
{
	struct State
	{
		std::string mName;
		unsigned int mIndex;
		std::vector<std::string> mEvents;
		std::vector<unsigned int> mEventStates;
		std::vector<unsigned int> mInputs;
		std::vector<unsigned int> mInputStates;
	};
}