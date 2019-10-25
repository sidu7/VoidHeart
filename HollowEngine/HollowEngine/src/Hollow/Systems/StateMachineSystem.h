#pragma once
#include "System.h"

namespace Hollow
{
	class HOLLOW_API StateMachineSystem : public System
	{
		REGISTERSYSTEM(StateMachineSystem, 1);
	public:
		virtual ~StateMachineSystem();
		void Update();
		void AddGameObject(GameObject* object);
	};
}