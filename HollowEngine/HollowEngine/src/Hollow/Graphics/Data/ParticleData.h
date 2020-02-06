#pragma once

namespace Hollow
{

#define MAX_PARTICLES_COUNT 2000000

	class ParticleEmitter;

	struct Particle
	{
		glm::vec3 mPosition;
		//glm::vec3 mScale;
		//glm::vec3 mRotation;
		//glm::vec3 mDirection;
		float mSpeed;
		float mLife;
		float mCurrentLife;
		glm::vec3 mDirection;
		//glm::vec3 mColor;
	};

	enum ParticleType
	{
		POINT,
		MODEL,
		PARTICLE_NUM
	};

	struct ParticleData
	{
		ParticleEmitter* emitter;
	};
}
