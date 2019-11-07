#pragma once
#include "System.h"

namespace Hollow
{
	class UIRenderSystem : public System
	{
		REGISTERSYSTEM(UIRenderSystem, 4);
	public:
		HOLLOW_API virtual ~UIRenderSystem() {}
		HOLLOW_API void Init();
		HOLLOW_API void AddGameObject(GameObject* object);
		HOLLOW_API void Update();
	};
}
