#pragma once
#include <vector>

namespace Hollow
{
	class System;

	class SystemManager
	{
	public:
		~SystemManager();
		static SystemManager& Instance()
		{
			static SystemManager instance;
			return instance;
		}

		void RegisterSystem(System* system)
		{
			mSystems.push_back(system);
		}

		void Update();

	private:
		std::vector<System*> mSystems;
	};
}