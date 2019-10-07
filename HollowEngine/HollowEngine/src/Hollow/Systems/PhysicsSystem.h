#pragma once
#include "System.h"
#include "Hollow/Physics/Broadphase/DynamicAABBTree.h"

namespace Hollow {
	class PhysicsSystem : public System
	{
		REGISTERSYSTEM(PhysicsSystem, 3)
	public:
		void Update(GameObject* gameobject);
		void AddGameObject(GameObject* object);

		// Data
		DynamicAABBTree mTree;
	};
};


