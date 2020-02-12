#pragma once
#include "System.h"

namespace Hollow
{
	class ParticleEmitter;

	/// <summary>
	/// Class ParticleSystem.
	/// Inherits the <see cref="System" /> class.
	/// </summary>
	/// <seealso cref="System" />
	class ParticleSystem : public System
	{
		/// <summary>
		/// Registers this System with the SystemManager
		/// </summary>
		REGISTERSYSTEM(ParticleSystem,3)
	public:
		/// <summary>
		/// Destructor for ParticleSystem
		/// </summary>
		HOLLOW_API virtual ~ParticleSystem() {}

		HOLLOW_API void Init();
		/// <summary>
		/// Updates the System every frame.
		/// </summary>
		HOLLOW_API void Update();
		/// <summary>
		/// Adds the game object.
		/// </summary>
		/// <param name="object">The object to add.</param>
		HOLLOW_API void AddGameObject(GameObject* object);

	private:
		void CalculateParticleMatrices(ParticleEmitter* emitter);
		void CalculateParticlePositions(ParticleEmitter* emitter, glm::vec3 center);
		void UpdateAttributes(ParticleEmitter* emitter);
	};
}