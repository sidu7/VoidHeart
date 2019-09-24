#pragma once

#include "System.h"

namespace Hollow
{
	class RenderSystem : public System
	{
		REGISTERSYSTEM(RenderSystem)
	public:
		virtual ~RenderSystem() {}
		void Update();
		void AddGameObject(GameObject* object);
	};
}