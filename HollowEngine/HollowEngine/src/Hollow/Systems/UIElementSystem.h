#pragma once
#include "System.h"

namespace Hollow
{
	class UIElementSystem : public System
	{
		REGISTERSYSTEM(UIElementSystem, 4);
	public:
		virtual ~UIElementSystem() {}
		void Init();
		void AddGameObject(GameObject* object);
		void Update();
	};
}
