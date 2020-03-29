#pragma once
#include "System.h"

namespace Hollow {
	class Camera;
	
	/// <summary>
	/// Class PhysicsSystem.
	/// Inherits the <see cref="System" /> class.
	/// </summary>
	/// <seealso cref="System" />
	class PhysicsSystem : public System
	{
		REGISTERSYSTEM(PhysicsSystem, 2)
	public:
		/// <summary>
		/// Updates the System every frame.
		/// </summary>
		HOLLOW_API void Update();
		/// <summary>
		/// Adds the game object.
		/// </summary>
		/// <param name="object">The object to add.</param>
		HOLLOW_API void AddGameObject(GameObject* object);
		/// <summary>
		/// Draws the debug displays for all the contacts.
		/// </summary>
		HOLLOW_API void DebugContacts();
		/// <summary>
		/// Called when a game object is deleted.
		/// </summary>
		/// <param name="pGameObject">The gameObject instance to delete.</param>
		HOLLOW_API void OnDeleteGameObject(GameObject* pGameObject) override;
		/// <summary>
		/// Called when all game objects have to be deleted.
		/// </summary>
		HOLLOW_API void OnDeleteAllGameObjects() override;
		
	private:
		void Step(float);
		void InterpolateState(float);

		bool nextStep;
		bool isApplyFriction = false;
		float accumulator;

		const int impulseIterations = 8;
		const float maxPossibleDeltaTime = 1.0f/60.0f;
		const float slop = -0.005f;
		const float mu = 0.5f;
		const float baumgarte = 0.15f;
		const float bias = 1.0f;
		const float proximityEpsilon = 0.00001f;
		
		glm::vec3 gravity = glm::vec3(0.0f, -9.81f, 0.0f);
	};
};


