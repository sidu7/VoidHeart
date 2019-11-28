#pragma once
#include "System.h"

namespace Hollow
{
	class UITextRenderSystem : public System
	{
		REGISTERSYSTEM(UITextRenderSystem, 4)
	public:
		HOLLOW_API void Init() override;
		HOLLOW_API void Update() override;
		HOLLOW_API void AddGameObject(GameObject* pGameObject) override;
		HOLLOW_API void HandleBroadcastEvent(GameEvent& event) override;
	private:
	};
}