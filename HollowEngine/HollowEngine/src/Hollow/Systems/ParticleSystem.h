#pragma once
#include "System.h"

namespace Hollow
{
	class ParticleEmitter;

	class HOLLOW_API ParticleSystem : public System
	{
		REGISTERSYSTEM(ParticleSystem,3)
	public:
		virtual ~ParticleSystem() {}
		void Update();
		void AddGameObject(GameObject* object);

	private:
		void CalculateParticleMatrices(ParticleEmitter* emitter);
		void CalculateParticlePositions(ParticleEmitter* emitter, glm::vec3 center);
	};
}