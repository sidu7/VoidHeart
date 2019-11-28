#pragma once
#include "System.h"

namespace Hollow
{
	class StateMachineSystem : public System
	{
		REGISTERSYSTEM(StateMachineSystem, 1);
	public:
		HOLLOW_API virtual ~StateMachineSystem();
		HOLLOW_API void Update() override;
		HOLLOW_API void AddGameObject(GameObject* object) override;
	};
}