#include "hollowpch.h"
#include "PhysicsSystem.h"
#include "Hollow/Components/Transform.h"
#include "Hollow/Components/Collider.h"

namespace Hollow
{
	PhysicsSystem PhysicsSystem::instance;

	void PhysicsSystem::AddGameObject(GameObject* object)
	{
		if (CheckComponents<Collider>(object))
			mTree.AddCollider(object->GetComponent<Collider>());
	}

	void PhysicsSystem::Update(GameObject* gameobject)
	{
		// balancing the tree
		mTree.Update();

		// finds out intersecting bounding boxes
		mTree.CalculatePairs();

		std::list < std::pair<Collider*, Collider*>>& pairs = mTree.GetPairs();

		HW_TRACE("{0}", pairs.size());
		
	}
}
