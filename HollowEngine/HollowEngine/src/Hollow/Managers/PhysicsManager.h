#pragma once

#include "Hollow/Core/Core.h"
#include "Hollow/Physics/Broadphase/DynamicAABBTree.h"
#include "Hollow/Physics/NarrowPhase/SAT.h"

namespace Hollow
{

	class Camera;
	class Collider;
	
	class HOLLOW_API PhysicsManager
	{
		SINGLETON(PhysicsManager)
	public:
		// Main Camera Object
		Camera* mRayCastCamera;

		// Data
		DynamicAABBTree mTree;

		// SAT
		SAT mSAT;


		void ApplyImpulse(GameObject* object, glm::vec3 impulse);
		Collider* castRay();
	};
}
