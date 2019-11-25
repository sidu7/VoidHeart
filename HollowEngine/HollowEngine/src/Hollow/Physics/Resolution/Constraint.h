#pragma once

#include "Hollow/Physics/NarrowPhase/Contact.h"
#include "Hollow/Components/Body.h"

#include "glm/glm.hpp"
#include "PhysicsDataTypes.h"

namespace Hollow {
	
	class Constraint
	{
		MassMatrix massMatrix;
	public:
		VelocityJacobian velocityJacobian;
		
		inline void EvaluateJacobian(Jacobian& jacobian, Contact* c, glm::vec3 dir)
		{
			jacobian.positionA = -dir;
			jacobian.positionB = dir;
 
			jacobian.orientationA = -glm::cross(c->rA, dir);
			jacobian.orientationB = glm::cross(c->rB, dir);
		}

		VelocityJacobian MassMatrixJacobianMult(MassMatrix& m, Jacobian& j)
		{
			VelocityJacobian result;
			result.velocityA = m.massA * j.positionA;
			result.velocityB = m.massB * j.positionB;
			result.angularVelocityA = m.inertiaA * j.orientationA;
			result.angularVelocityB = m.inertiaB * j.orientationB;

			return result;
		}

		float JacobianJacobianMult(Jacobian& j1, VelocityJacobian& j2)
		{
			float sum = 0.0f;
			sum += glm::dot(j1.positionA, j2.velocityA);
			sum += glm::dot(j1.positionB, j2.velocityB);
			sum += glm::dot(j1.orientationA, j2.angularVelocityA);
			sum += glm::dot(j1.orientationB, j2.angularVelocityB);

			return sum;
		}
		
		VelocityJacobian JacobianScalarMult(VelocityJacobian& j, float val)
		{
			VelocityJacobian result;
			result.velocityA = j.velocityA* val;
			result.velocityB = j.velocityB * val;
			result.angularVelocityA = j.angularVelocityA* val;
			result.angularVelocityB	 = j.angularVelocityB * val;
			return result;
		}
		
		// makes the mass matrix and calculates the effective mass
		inline void CalculateMassMatrixInv(Body* bodyA, Body* bodyB)
		{
			for (int i = 0; i < 3; ++i) {
				for (int j = 0; j < 3; ++j) {
					if(i==j)
					{
						massMatrix.massA[i][j] = bodyA->mInverseMass;
						massMatrix.massB[i][j] = bodyB->mInverseMass;
					}
					else
					{
						massMatrix.massA[i][j] = 0;
						massMatrix.massB[i][j] = 0;
					}				
				}
			}
			massMatrix.inertiaA = bodyA->mWorldInertiaInverse;
			massMatrix.inertiaB = bodyB->mWorldInertiaInverse;
		}

		inline void EvaluateVelocityJacobian(Body* bodyA, Body* bodyB)
		{
			velocityJacobian.velocityA = bodyA->mVelocity;
			velocityJacobian.velocityB = bodyB->mVelocity;

			velocityJacobian.angularVelocityA = bodyA->mAngularVelocity;
			velocityJacobian.angularVelocityB = bodyB->mAngularVelocity;
		}

		void CalculateEffectiveMass(VelocityJacobian& mMatxj, Jacobian& jacobian, float& effectiveMass)
		{
			mMatxj = MassMatrixJacobianMult(massMatrix, jacobian);
			effectiveMass = 1.0f / JacobianJacobianMult(jacobian, mMatxj);
		}
		
		inline void ApplyImpulse(Body* bodyA, Body* bodyB, VelocityJacobian& mMatxj, float impulse)
		{
			VelocityJacobian const& dvj = JacobianScalarMult(mMatxj, impulse);

			if (bodyA->mBodyType != Body::RigidbodyType::KINEMATIC) {
				bodyA->mVelocity += dvj.velocityA;
				bodyA->mAngularVelocity += dvj.angularVelocityA;
			}

			if (bodyB->mBodyType != Body::RigidbodyType::KINEMATIC) {
				bodyB->mVelocity += dvj.velocityB;
				bodyB->mAngularVelocity += dvj.angularVelocityB;
			}
		}

	};

}
