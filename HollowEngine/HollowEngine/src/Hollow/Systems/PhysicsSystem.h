#pragma once
#include "System.h"

namespace Hollow {
	class Camera;
	
	class PhysicsSystem : public System
	{
		REGISTERSYSTEM(PhysicsSystem, 2)
	public:
		HOLLOW_API void Update();
		HOLLOW_API void AddGameObject(GameObject* object);
		HOLLOW_API void DebugContacts();
		
	private:
		void Step(float);
		void InterpolateState(float);

		bool isPaused = false;
		bool nextStep;
		bool isApplyFriction = false;
		float accumulator;

		const int impulseIterations = 8;
		const float maxPossibleDeltaTime = 1.0f/60.0f;
		const float slop = -0.005f;
		const float mu = 0.005f;
		const float baumgarte = 0.15f;
		const float bias = 1.0f;
		const float proximityEpsilon = 0.00001f;
		
		glm::vec3 gravity = glm::vec3(0.0f, -9.81f, 0.0f);
	};
};


