#pragma once

#include "Hollow/Core/Core.h"
#include "Hollow/Physics/Broadphase/DynamicAABBTree.h"
#include "Hollow/Physics/NarrowPhase/SAT.h"

namespace Hollow
{

	class Camera;
	class Collider;
	
	class PhysicsManager
	{
		SINGLETON(PhysicsManager)
	public:

		// Data
		DynamicAABBTree mTree;

		// SAT
		SAT mSAT;


		HOLLOW_API  void ApplyImpulse(GameObject* object, glm::vec3 impulse);
		HOLLOW_API  GameObject* CastRay();
	};
}
