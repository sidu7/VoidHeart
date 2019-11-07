#pragma once
#include "System.h"

namespace Hollow
{
	class LightSystem : public System
	{
		REGISTERSYSTEM(LightSystem,3)
	public:
		HOLLOW_API virtual ~LightSystem() {}
		HOLLOW_API void Update();
		HOLLOW_API void AddGameObject(GameObject* object);
	};
}