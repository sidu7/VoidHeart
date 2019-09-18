#pragma once
#include <vector>

namespace Hollow
{
	class System;

	class SystemManager
	{
		SINGLETON(SystemManager)
	public:
		~SystemManager();

		void RegisterSystem(System* system)
		{
			mSystems.push_back(system);
		}

		void Update();

	private:
		std::vector<System*> mSystems;
	};
}