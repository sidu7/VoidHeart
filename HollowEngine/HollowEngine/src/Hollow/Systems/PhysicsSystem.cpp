#include <hollowpch.h>
#include "PhysicsSystem.h"
#include "Hollow/Components/Transform.h"
#include "Hollow/Components/Body.h"
#include "Hollow/Components/Collider.h"
#include "Hollow/Managers/FrameRateController.h"
#include "Hollow/Managers/InputManager.h"
#include "Hollow/Components/Camera.h"
#include "Hollow/Managers/DebugDrawManager.h"

#include <glm/gtc/matrix_transform.hpp>
#include <stack>

namespace Hollow
{
	PhysicsSystem PhysicsSystem::instance;

	void PhysicsSystem::CheckCameraComponentAndAdd(GameObject* object)
	{
		Camera* c = object->GetComponent<Camera>();
		if (c)
		{
			mRayCastCamera = c;
		}
	}

	
	void PhysicsSystem::AddGameObject(GameObject* object)
	{
		CheckCameraComponentAndAdd(object);
		
		if (CheckComponents<Collider>(object)) {
			// Collider Init
			Collider* pCol = object->GetComponent<Collider>();
			
			pCol->mpTr = static_cast<Transform*>(object->GetComponent<Transform>());
			pCol->mpBody = static_cast<Body*>(object->GetComponent<Body>());

			// TODO write inertia formula for spheres
			glm::mat3 inertia = glm::mat3(0.0f);
			if (pCol->mpShape->mType == AABB) {
				inertia[0][0] = pCol->mpBody->mMass / 12.0f * (pCol->mpTr->mScale.y * pCol->mpTr->mScale.y + pCol->mpTr->mScale.z * pCol->mpTr->mScale.z);
				inertia[1][1] = pCol->mpBody->mMass / 12.0f * (pCol->mpTr->mScale.x * pCol->mpTr->mScale.x + pCol->mpTr->mScale.z * pCol->mpTr->mScale.z);
				inertia[2][2] = pCol->mpBody->mMass / 12.0f * (pCol->mpTr->mScale.y * pCol->mpTr->mScale.y + pCol->mpTr->mScale.x * pCol->mpTr->mScale.x);
			}
			if (pCol->mpBody->mInverseMass == 0.0f)
				pCol->mpBody->mLocalInertiaInverse = glm::mat3(0);
			else
				pCol->mpBody->mLocalInertiaInverse = glm::inverse(inertia);
			
			
			pCol->mpBody->mPosition = pCol->mpTr->mPosition;
			pCol->mpBody->mPreviousPosition = pCol->mpTr->mPosition;
			pCol->mpBody->mQuaternion = pCol->mpTr->mQuaternion;
			pCol->mpBody->mRotationMatrix = glm::toMat3(pCol->mpBody->mQuaternion);
			
			// update local shape (0.5f because we are updating half extents)
			static_cast<ShapeAABB*>(pCol->mpLocalShape)->mMin = -0.5f *(pCol->mpTr->mScale);
			static_cast<ShapeAABB*>(pCol->mpLocalShape)->mMax = 0.5f * (pCol->mpTr->mScale);

			// Collider added to Dynamic BVH
			mTree.AddCollider(pCol);
		}
	}

	void PhysicsSystem::DebugContacts()
	{
		ImGui::Begin("Contacts");
		{
			for (auto points : *mSAT.mContacts)
			{
				for (int i = 0; i < points->contactPoints.size(); i++)
				{
					ImGui::Text("Point location- %f , %f , %f ", points->contactPoints[i].point.x, points->contactPoints[i].point.y, points->contactPoints[i].point.z);
					ImGui::Text("Penetration depth - %f", points->contactPoints[i].penetrationDepth);
				}
			}
		}
		ImGui::End();
	}

	Collider* PhysicsSystem::castRay()
	{
		std::pair<float, float> mouseXY = Hollow::InputManager::Instance().GetMousePosition();
		float x = 2.0f * (mouseXY.first + 0.5f) / 1280.0f - 1.0f,
			y = 1.0f- 2.0f * (mouseXY.second + 0.5f) / 720.0f;
		
		Ray r;
		r.origin = mRayCastCamera->mPosition; // -mRayCastCamera->mFront;
		//r.direction = mRayCastCamera->mFront + x * mRayCastCamera->mRight + y * mRayCastCamera->mUp;

		glm::mat4 persp = glm::perspective(glm::radians(mRayCastCamera->mZoom), 16.0f/9.0f, mRayCastCamera->mNear, mRayCastCamera->mFar);
		//glm::mat4 ortho = glm::ortho(0.0f, 1280.0f, 720.0f, 0.0f, mRayCastCamera->mNear, mRayCastCamera->mFar);
		glm::mat4 view = glm::lookAt(mRayCastCamera->mPosition, mRayCastCamera->mPosition + mRayCastCamera->mFront, mRayCastCamera->mUp);

		// NDC to camera space
		glm::vec4 ray_clip = glm::vec4(x, y, -1.0f, 1.0f);
		glm::vec4 ray_eye = glm::inverse(persp) * ray_clip;

		// camera to world space
		ray_eye = glm::vec4(ray_eye.x, ray_eye.y , -1.0, 0.0);
		glm::vec3 direction = glm::vec3(glm::inverse(view) * ray_eye);

		direction = glm::normalize(direction);

		r.direction = direction;
		
		DebugDrawManager::Instance().DebugRay(r.origin, r.direction);
		
		IntersectionData id, closest;
		closest.object = nullptr;
		closest.depth = std::numeric_limits<float>::max();

		/*for (int i = 0; i < mGameObjects.size(); ++i) {
			Shape* shape = mGameObjects[i]->GetComponent<Collider>()->mpShape;
			glm::mat3& rot = mGameObjects[i]->GetComponent<Body>()->mRotationMatrix;
			glm::vec3 extents = mGameObjects[i]->GetComponent<Transform>()->mScale;
			
			if (shape->TestRay(r, id, rot, extents)) {
				if (id.depth < closest.depth) {
					closest = id;
				}
			}
		}*/

		Node* root = mTree.GetRoot();

		std::stack<Node*> s;
		Node* curr = root;

		// inorder traversal for printing
		while ((curr != NULL && curr->aabb->TestRay(r, id)) || s.empty() == false)
		{
			
			while (curr != NULL && curr->aabb->TestRay(r, id))
			{
				s.push(curr);
				curr = curr->left;
			}

			curr = s.top();
			s.pop();

			if(curr->IsLeaf())
			{
				// cannot use curr->aabb because the mpOwnerCollider in the shape would always be null
				Shape* shape = static_cast<Collider*>(curr->mClientData)->mpShape;
				
				glm::mat3& rot = static_cast<Collider*>(curr->mClientData)->mpBody->mRotationMatrix;
				glm::vec3 extents = static_cast<Collider*>(curr->mClientData)->mpTr->mScale;
				if (shape->TestRay(r, id, rot, extents)) {
					if (id.depth < closest.depth) {
						closest = id;
					}
				}
			}

			curr = curr->right;
		}

		if (closest.object == nullptr) { return nullptr; }
		
		return static_cast<Collider*>(closest.object);
	}

	void PhysicsSystem::Step(float fixedDeltaTime)
	{
		for (unsigned int i = 0; i < mGameObjects.size(); ++i)
		{
			Collider* pCol = mGameObjects[i]->GetComponent<Collider>();
			glm::vec3 extents = static_cast<ShapeAABB*>(pCol->mpLocalShape)->GetHalfExtents();
			glm::vec3 x = glm::vec3(extents.x, 0.0f, 0.0f);
			glm::vec3 y = glm::vec3(0.0f, extents.y, 0.0f);
			glm::vec3 z = glm::vec3(0.0f, 0.0f, extents.z);
			glm::vec3 rotatedExtents = abs((pCol->mpBody->mRotationMatrix) * x) +
				abs((pCol->mpBody->mRotationMatrix) * y) +
				abs((pCol->mpBody->mRotationMatrix) * z);

			// based on normalized body vertices
			static_cast<ShapeAABB*>(pCol->mpShape)->mMin = glm::vec3(-rotatedExtents.x, -rotatedExtents.y, -rotatedExtents.z) + pCol->mpBody->mPosition;
			static_cast<ShapeAABB*>(pCol->mpShape)->mMax = glm::vec3(rotatedExtents.x, rotatedExtents.y, rotatedExtents.z) + pCol->mpBody->mPosition;
		}

		// balancing the tree
		mTree.Update();

		// finds out intersecting bounding boxes
		mTree.CalculatePairs();

		std::list < std::pair<Collider*, Collider*>>& pairs = mTree.GetPairs();

		for (auto& pair : pairs) {
			// perform the SAT intersection test
			mSAT.TestIntersection3D(pair.first, pair.second);
		}

		//HW_TRACE("{0}", mSAT.mContacts->size());

		for (auto go : mGameObjects)
		{
			Body* pBody = static_cast<Body*>(go->GetComponent<Body>());

			if (pBody->mInverseMass == 0.0f)
				continue;

			// compute acceleration
			glm::vec3 acc = pBody->mTotalForce * pBody->mInverseMass;
			acc += gravity;
			glm::vec3 alpha = pBody->mTotalTorque * pBody->mWorldInertiaInverse;

			// integrate acc into the velocity
			pBody->mVelocity += acc * fixedDeltaTime;
			pBody->mAngularVelocity += alpha * fixedDeltaTime;

			// set forces to zero
			pBody->mTotalForce = glm::vec3(0);
			pBody->mTotalTorque = glm::vec3(0);
		}

		// TODO remove loops and replace with hashmap
		if (!mSAT.mPrevContacts->empty())
		{
			for (auto itOld : *mSAT.mPrevContacts)
			{
				for (auto itNew : *mSAT.mContacts)
				{
					if (((*itNew).bodyA == (*itOld).bodyA &&
						(*itNew).bodyB == (*itOld).bodyB)) {

						// iterate through contact points
						for (int j = 0; j < (*itOld).contactPoints.size(); ++j)
						{
							Contact& cOld = (*itOld).contactPoints[j];

							for (int k = 0; k < (*itNew).contactPoints.size(); ++k) {
								Contact& cNew = (*itNew).contactPoints[k];

								if (glm::distance2(cNew.point, cOld.point) < proximityEpsilon)
								{
									cNew.normalImpulseSum = cOld.normalImpulseSum * bias;
									cNew.tangentImpulseSum1 = cOld.tangentImpulseSum1 * bias;
									cNew.tangentImpulseSum2 = cOld.tangentImpulseSum2 * bias;

									// apply old impulse as warm start
									(itNew)->constraint.EvaluateJacobian(&cNew, itNew->collisionNormal);
									itNew->constraint.ApplyImpulse(itNew->bodyA, itNew->bodyB, cNew.normalImpulseSum);

									itNew->constraint.EvaluateJacobian(&cNew, itNew->t0);
									itNew->constraint.ApplyImpulse(itNew->bodyA, itNew->bodyB, cNew.tangentImpulseSum1);

									itNew->constraint.EvaluateJacobian(&cNew, itNew->t1);
									itNew->constraint.ApplyImpulse(itNew->bodyA, itNew->bodyB, cNew.tangentImpulseSum2);
								}
							}

						}

					}
				}
			}
		}

		//DebugContacts();
		
		for (int i = 0; i < impulseIterations; ++i) {
			for (auto c : *mSAT.mContacts) {

				//std::cout << c->contactPoints[0]->normalImpulseSum << std::endl;
				int pointCount = c->contactPoints.size();

				for (int j = 0; j < pointCount; ++j) {

					c->constraint.EvaluateVelocityVector(c->bodyA, c->bodyB);

					//===== solve for normal constraint
					c->constraint.EvaluateJacobian(&c->contactPoints[j], c->collisionNormal);

					float effMass = 1.0f / (c->constraint.jacobian * c->constraint.massMatrixInverse * c->constraint.jacobianT);

					// bias value
					float b = baumgarte / fixedDeltaTime * std::min(c->contactPoints[j].penetrationDepth - slop, 0.0f);
					//float b = 0.1f / fixedDeltaTime * c->contactPoints[j]->penetrationDepth;

					float lambda = -effMass * (c->constraint.jacobian * c->constraint.velocityMatrix + b);
					float origNormalImpulseSum = c->contactPoints[j].normalImpulseSum;

					c->contactPoints[j].normalImpulseSum += lambda;
					c->contactPoints[j].normalImpulseSum =
						glm::clamp(c->contactPoints[j].normalImpulseSum, 0.0f, std::numeric_limits<float>::infinity());

					float deltaLambda = c->contactPoints[j].normalImpulseSum - origNormalImpulseSum;

					c->constraint.ApplyImpulse(c->bodyA, c->bodyB, deltaLambda);

					if (isApplyFriction) {
						//float nLambda = c->contactPoints[j]->normalImpulseSum;
						float nLambda = -gravity.y / pointCount;

						// calculate tangents (Erin Catto's code)
						glm::vec3 t0, t1;

						if (abs(c->collisionNormal.x) >= 0.57735f)
							t0 = glm::normalize(glm::vec3(c->collisionNormal.y, -c->collisionNormal.x, 0.0f));
						else
							t0 = glm::normalize(glm::vec3(0.0f, c->collisionNormal.z, -c->collisionNormal.y));
						t1 = glm::cross(c->collisionNormal, t0);

						//==== solve for tangent 0
						c->constraint.EvaluateJacobian(&c->contactPoints[j], t0);

						effMass = 1.0f / (c->constraint.jacobian * c->constraint.massMatrixInverse * c->constraint.jacobianT);

						lambda = -effMass * (c->constraint.jacobian * c->constraint.velocityMatrix + 0.0f);

						float origTangent0ImpulseSum = c->contactPoints[j].tangentImpulseSum1;

						c->contactPoints[j].tangentImpulseSum1 += lambda;
						c->contactPoints[j].tangentImpulseSum1 =
							glm::clamp(c->contactPoints[j].tangentImpulseSum1, -mu * nLambda, mu * nLambda);

						deltaLambda = c->contactPoints[j].tangentImpulseSum1 - origTangent0ImpulseSum;

						c->constraint.ApplyImpulse(c->bodyA, c->bodyB, deltaLambda);

						//==== solve for tangent 1
						c->constraint.EvaluateJacobian(&c->contactPoints[j], t1);

						effMass = 1.0f / (c->constraint.jacobian * c->constraint.massMatrixInverse * c->constraint.jacobianT);

						lambda = -effMass * (c->constraint.jacobian * c->constraint.velocityMatrix + 0.0f);
						float origTangent1ImpulseSum = c->contactPoints[j].tangentImpulseSum2;

						c->contactPoints[j].tangentImpulseSum2 += lambda;
						c->contactPoints[j].tangentImpulseSum2 =
							glm::clamp(c->contactPoints[j].tangentImpulseSum2, -mu * nLambda, mu * nLambda);

						deltaLambda = c->contactPoints[j].tangentImpulseSum2 - origTangent1ImpulseSum;

						c->constraint.ApplyImpulse(c->bodyA, c->bodyB, deltaLambda);
					}
				}
			}
		}

		mSAT.CopyContacts();
		
		for (auto go : mGameObjects)
		{
			Body* pBody = static_cast<Body*>(go->GetComponent<Body>());
			// save current position
			pBody->mPreviousPosition = pBody->mPosition;

			// integrate the position
			pBody->mPosition += pBody->mVelocity * fixedDeltaTime;

			pBody->mPreviousQuaternion = pBody->mQuaternion;

			// integrate the orientation
			glm::fquat newQuat = 0.5f * (pBody->mAngularVelocity) * pBody->mQuaternion * fixedDeltaTime;
			pBody->mQuaternion *= newQuat;
		}
	}

	void PhysicsSystem::InterpolateState(float blendingFactor)
	{
		for (auto go : mGameObjects)
		{
			Body* pBody = static_cast<Body*>(go->GetComponent<Body>());
			Transform* pTr = static_cast<Transform*>(go->GetComponent<Transform>());

			pTr->mPosition.x = glm::mix(pBody->mPreviousPosition.x, pBody->mPosition.x, blendingFactor);
			pTr->mPosition.y = glm::mix(pBody->mPreviousPosition.y, pBody->mPosition.y, blendingFactor);
			pTr->mPosition.z = glm::mix(pBody->mPreviousPosition.z, pBody->mPosition.z, blendingFactor);


			// if there is a significant change in position or orientation only then update transformation matrix
			if (glm::length2(pBody->mVelocity) > 0.0001f || glm::length2(pBody->mAngularVelocity) > 0.0001f) {
				pTr->dirtyBit = true;
			}

			pBody->mQuaternion = glm::normalize(pBody->mQuaternion);
			pTr->mQuaternion = glm::slerp(pBody->mPreviousQuaternion, pBody->mQuaternion, blendingFactor);
			
			pBody->mRotationMatrix = glm::toMat3(pBody->mQuaternion);
			pBody->mWorldInertiaInverse =
				pBody->mRotationMatrix *
				pBody->mLocalInertiaInverse *
				glm::transpose(pBody->mRotationMatrix);
			//pTr->mRotation = pBody->mRotationMatrix;
		}
	}

	void PhysicsSystem::Update()
	{
		isPaused = InputManager::Instance().IsKeyTriggered(SDL_SCANCODE_P) == true ? !isPaused : isPaused;
		
		nextStep = InputManager::Instance().IsKeyTriggered(SDL_SCANCODE_SPACE);

		ImGui::Begin("RayCast Result");
		ImGui::Text("Mouse X : %f", InputManager::Instance().GetMouseX());
		ImGui::Text("Mouse Y : %f", InputManager::Instance().GetMouseY());
		if(InputManager::Instance().IsKeyPressed(SDL_SCANCODE_R))
		{
			Collider* pCol = castRay();

			if(pCol)
			{
				ImGui::InputFloat3("Collider present", &pCol->mpTr->mScale[0]);
			}
		}
		ImGui::End();
		
		//================Physics Update======================
		float dt = FrameRateController::Instance().GetFrameTime();


		if (!isPaused) {
			accumulator += dt;
			while (accumulator > maxPossibleDeltaTime) {
				//{
					//Timer t;
				Step(maxPossibleDeltaTime);
				//}
				accumulator -= maxPossibleDeltaTime;
			}
			InterpolateState(accumulator / maxPossibleDeltaTime);
		}
		else if (nextStep) {
			accumulator = 0.0f;
			Step(maxPossibleDeltaTime);
			InterpolateState(1.0f);
		}
		
	}
}
