#pragma once
#include <unordered_map>
#include <iostream>

namespace Hollow
{
	class GameMetaData
	{
		SINGLETON(GameMetaData);
	public:
		std::unordered_map<std::string, int> mMapOfGameObjectTypes;
		std::unordered_map<std::string, int> mMapOfGameEventTypes;
		
		/// <summary>
		/// Initializes GameMetaData which contains the CollisionMasks.
		/// </summary>
		HOLLOW_API void Init();
	};
}