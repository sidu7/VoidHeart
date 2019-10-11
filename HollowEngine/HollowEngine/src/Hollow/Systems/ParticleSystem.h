#pragma once
#include "System.h"

namespace Hollow
{
	class HOLLOW_API ParticleSystem : public System
	{
		REGISTERSYSTEM(ParticleSystem,3)
	public:
		virtual ~ParticleSystem() {}
		void Update();
		void AddGameObject(GameObject* object);
	};
}