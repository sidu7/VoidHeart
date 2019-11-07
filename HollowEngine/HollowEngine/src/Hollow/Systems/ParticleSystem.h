#pragma once
#include "System.h"

namespace Hollow
{
	class ParticleEmitter;

	class ParticleSystem : public System
	{
		REGISTERSYSTEM(ParticleSystem,3)
	public:
		HOLLOW_API virtual ~ParticleSystem() {}
		HOLLOW_API void Update();
		HOLLOW_API void AddGameObject(GameObject* object);

	private:
		void CalculateParticleMatrices(ParticleEmitter* emitter);
		void CalculateParticlePositions(ParticleEmitter* emitter, glm::vec3 center);
		void UpdateAttributes(ParticleEmitter* emitter);
	};
}