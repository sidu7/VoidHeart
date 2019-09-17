#pragma once
#include "System.h"
#include <iostream>

namespace Hollow
{
	class TempSystem : public System
	{
	private:
		static TempSystem instance;
		TempSystem() : System(this) {
			std::cout << "TempSystem registered" << std::endl;
		}
	public:		
		~TempSystem() {}
		void Update();
		void AddGameObject(GameObject* object);
	};

	TempSystem TempSystem::instance;
}