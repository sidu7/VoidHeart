#pragma once
#include "System.h"

namespace Hollow
{
	class UIRenderSystem : public System
	{
		REGISTERSYSTEM(UIRenderSystem, 4);
	public:
		HOLLOW_API virtual ~UIRenderSystem() {}
		HOLLOW_API void Init() override;
		HOLLOW_API void AddGameObject(GameObject* object) override;
		HOLLOW_API void Update() override;
	};
}
