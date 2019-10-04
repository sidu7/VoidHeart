#pragma once

#include "System.h"

namespace Hollow
{
	class RenderSystem : public System
	{
		REGISTERSYSTEM(RenderSystem,3)
	public:
		virtual ~RenderSystem() {}
		void Update();
		void AddGameObject(GameObject* object);
	};
}