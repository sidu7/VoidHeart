#pragma once
#include "System.h"
#include "Hollow/Physics/Broadphase/DynamicAABBTree.h"
#include "Hollow/Physics/NarrowPhase/SAT.h"

namespace Hollow {
	class PhysicsSystem : public System
	{
		REGISTERSYSTEM(PhysicsSystem, 2)
	public:
		void Step(float);
		void Update();
		void AddGameObject(GameObject* object);
		void InterpolateState(float);
		// Data
		DynamicAABBTree mTree;

		// SAT
		SAT mSAT;

	private:
		bool isPaused;
		bool nextStep;
		bool isApplyFriction = true;
		float accumulator;

		const int impulseIterations = 8;
		const float maxPossibleDeltaTime = 1.0f/60.0f;
		const float slop = -0.005f;
		const float mu = 0.2f;
		const float baumgarte = 0.1f;
		const float bias = 1.0f;
		const float proximityEpsilon = 0.001f;
		
		glm::vec3 gravity = glm::vec3(0.0f, -9.81f, 0.0f);
	};
};


