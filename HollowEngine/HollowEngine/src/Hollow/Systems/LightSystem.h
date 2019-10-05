#pragma once
#include "System.h"

namespace Hollow
{
	class LightSystem : public System
	{
		REGISTERSYSTEM(LightSystem,3)
	public:
		virtual ~LightSystem() {}
		void Update(GameObject* gameobject);
		void AddGameObject(GameObject* object);
	};
}