#pragma once
#include "System.h"
#include <iostream>

namespace Hollow
{
	class TempSystem : public System
	{
		REGISTERSYSTEM(TempSystem)
	public:		
		~TempSystem() {}
		void Update();
		void AddGameObject(GameObject* object);
	};
}