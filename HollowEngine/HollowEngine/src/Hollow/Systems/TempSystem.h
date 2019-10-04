#pragma once
#include "System.h"
#include <iostream>

namespace Hollow
{
	class TempSystem : public System
	{
		REGISTERSYSTEM(TempSystem,0)
	public:		
		
		void Update();
		void AddGameObject(GameObject* object);
	};
}