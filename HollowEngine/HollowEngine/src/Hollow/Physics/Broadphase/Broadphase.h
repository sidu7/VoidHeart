#pragma once

#include <vector>

#include "Hollow/Components/Collider.h"
namespace Hollow {
	typedef std::pair<Collider*, Hollow::Collider*> ColliderPair;

	class Broadphase
	{
	public:

		virtual void AddCollider(Collider*) = 0;

		virtual void Update() = 0;

		//virtual const std::list<ColliderPair>& CalculatePairs() = 0;
		virtual void CalculatePairs() = 0;

		virtual void CollisionQuery(const Shape& shape, std::vector<Collider*>& colliders) = 0;
	};

}