#pragma once

#include "Hollow/Core/Core.h"
#include "Hollow/Systems/System.h"

namespace Hollow {
	class ScriptingSystem :public System
	{
		REGISTERSYSTEM(ScriptingSystem, 2)
	public:
		HOLLOW_API void Update() override;
		HOLLOW_API void AddGameObject(GameObject* pGameObject) override;
	};
}
