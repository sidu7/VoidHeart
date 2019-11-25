#pragma once
#include "Hollow/Common.h"
#include <unordered_map>
#include <iostream>

namespace Hollow
{
	class GameObjectMetaData
	{
		SINGLETON(GameObjectMetaData);
	public:
		std::unordered_map<std::string, int> mMapOfGameObjectTypes;

		HOLLOW_API void Init();
	};
}