#pragma once

namespace Hollow
{
	// generates a unique ID for a specific type.
	template <typename T>
	unsigned int GenerateUniqueID()
	{
		static unsigned int uniqueID = 0;
		++uniqueID;
		return uniqueID;
	}
}