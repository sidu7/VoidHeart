#pragma once

#include "System.h"

namespace Hollow
{
	class RenderSystem : public System
	{
		REGISTERSYSTEM(RenderSystem,4)
	public:
		HOLLOW_API virtual ~RenderSystem() {}
		HOLLOW_API void Update();
		HOLLOW_API void AddGameObject(GameObject* object);
	};
}