#pragma once
#include "System.h"

namespace Hollow
{
	class UIRenderSystem : public System
	{
		REGISTERSYSTEM(UIRenderSystem, 4);
	public:
		virtual ~UIRenderSystem() {}
		void Init();
		void AddGameObject(GameObject* object);
		void Update();
	};
}
