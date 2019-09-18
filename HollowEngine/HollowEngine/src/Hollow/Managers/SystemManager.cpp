#include <hollowpch.h>

#include "SystemManager.h"
#include "../Systems/System.h"
#include "../Log.h"

namespace Hollow
{
	SystemManager::~SystemManager()
	{
		mSystems.clear();
	}

	void SystemManager::Update()
	{
		for (unsigned int i = 0; i < mSystems.size(); ++i)
		{
			mSystems[i]->Update();
		}
	}
}
