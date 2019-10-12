#pragma once
#include "Hollow/Components/Body.h"
#include "Contact.h"
#include "Hollow/Physics/Resolution/Constraint.h"

namespace Hollow {
	class ContactManifold {
	public:
		std::vector<Contact> contactPoints;

		Body* bodyA, * bodyB;
		Constraint constraint;

		glm::vec3 collisionNormal;
		glm::vec3 t0, t1;

		ContactManifold() : collisionNormal(0), t0(0), t1(0)
		{}
		EIGEN_MAKE_ALIGNED_OPERATOR_NEW
			void SetupGroundConstraint()
		{
			// calculate tangents (Erin Catto's code)
			if (abs(collisionNormal.x) >= 0.57735f)
				t0 = glm::normalize(glm::vec3(collisionNormal.y, -collisionNormal.x, 0.0f));
			else
				t0 = glm::normalize(glm::vec3(0.0f, collisionNormal.z, -collisionNormal.y));
			t1 = glm::cross(collisionNormal, t0);

			constraint.CalculateMassMatrixInv(bodyA, bodyB);
		}

		ContactManifold(ContactManifold& cm)
		{
			bodyA = cm.bodyA;
			bodyB = cm.bodyB;

			collisionNormal = cm.collisionNormal;
			constraint = cm.constraint;

			for (auto cp : cm.contactPoints)
			{
				Contact c(cp);
				contactPoints.emplace_back(c);
			}
		}
	};

}