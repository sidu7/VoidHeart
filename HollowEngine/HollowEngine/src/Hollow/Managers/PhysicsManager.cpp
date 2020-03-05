#include <hollowpch.h>
#include "Hollow/Common.h"
#include "PhysicsManager.h"
#include "InputManager.h"

#include "DebugDrawManager.h"

#include "Hollow/Components/Transform.h"
#include "Hollow/Physics/NarrowPhase/SAT.h"
#include "Hollow/Physics/Broadphase/DynamicAABBTree.h"
#include "Hollow/Components/Camera.h"

#include "RenderManager.h"
#include "Hollow/Core/GameObject.h"
#include "Hollow/Core/GameMetaData.h"

namespace Hollow {

	void PhysicsManager::CleanUp()
	{
		mSAT.ResetContacts();
		mTree.DeleteTree();
		mRigidbodyTypesMap.clear();
		mCollisionMask.clear();
	}

	void PhysicsManager::ApplyAngularImpulse(GameObject* object, glm::vec3 impulse)
	{
		Body* pBody = object->GetComponent<Body>();
		pBody->mAngularVelocity += -glm::vec3(-impulse.z,0.0f,impulse.x) * pBody->mInverseMass;
		//ApplyLinearImpulse(object, impulse);
	}

	void PhysicsManager::ApplyLinearImpulse(GameObject* object, glm::vec3 impulse)
	{
		Body* pBody = object->GetComponent<Body>();
		pBody->mVelocity += impulse * pBody->mInverseMass;
		//pBody->mVelocity.x = pBody->mVelocity.x - 0.005 * pBody->mVelocity.x;
		//pBody->mVelocity.z = pBody->mVelocity.z - 0.005 * pBody->mVelocity.z;
		//pBody->mAngularVelocity = pBody->mAngularVelocity - 0.05 * pBody->mAngularVelocity;
	}
	
	GameObject* PhysicsManager::CastRay()
	{
		if (ImGui::IsWindowFocused(ImGuiFocusedFlags_AnyWindow))
		{
			return nullptr;
		}
		glm::ivec2 mouseXY = InputManager::Instance().GetMousePosition();
		float x = 2.0f * (mouseXY.x + 0.5f) / 1280.0f - 1.0f,
			y = 1.0f - 2.0f * (mouseXY.y + 0.5f) / 720.0f;

		auto& camera = RenderManager::Instance().mMainCamera;
		
		Ray r;
		r.origin = camera.mEyePosition; // -mRayCastCamera->mFront;

		// NDC to camera space
		glm::vec4 ray_clip = glm::vec4(x, y, -1.0f, 1.0f);
		glm::vec4 ray_eye = glm::inverse(camera.mProjectionMatrix) * ray_clip;

		// camera to world space
		ray_eye = glm::vec4(ray_eye.x, ray_eye.y, -1.0, 0.0);
		glm::vec3 direction = glm::vec3(glm::inverse(camera.mViewMatrix) * ray_eye);

		direction = glm::normalize(direction);

		r.direction = direction;

		//DebugDrawManager::Instance().DebugRay(r.origin, r.direction);

		IntersectionData id, closest;
		closest.object = nullptr;
		closest.depth = std::numeric_limits<float>::max();

		// simple but inefficient solution
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

			if (curr->IsLeaf())
			{
				Collider* pCol = static_cast<Collider*>(curr->mClientData);
				
				// cannot use curr->aabb because the mpOwnerCollider in the shape would always be null
				Shape* shape = pCol->mpShape;

				glm::mat3 rot;
				glm::vec3 extents = pCol->mpTr->mScale;

				rot = (pCol->mIsTrigger) ? glm::mat3(1.0f) : pCol->mpBody->mRotationMatrix;
				
								
				if (shape->TestRay(r, id, rot, extents)) 
				{
					if (id.depth < closest.depth) 
					{
						closest = id;
					}
				}
				
			}

			curr = curr->right;
		}

		if (closest.object == nullptr) { return nullptr; }

		return static_cast<Collider*>(closest.object)->mpOwner;
	
	}

	void PhysicsManager::DebugTreeNullNode(Node* key, int nullcount, FILE* stream)
	{
		long long nodeAdd = reinterpret_cast<long long>(key);
		fprintf(stream, "    null%d [shape=point];\n", nullcount);
		fprintf(stream, "    %u -> null%d;\n", key->ID, nullcount);
	}

	void PhysicsManager::DebugTreeNode(Node* node, FILE* stream)
	{
		static int nullcount = 0;

		if (node->left)
		{
			fprintf(stream, "    %u -> %u;\n", node->ID, node->left->ID);
			DebugTreeNode(node->left, stream);
		}
		else
			DebugTreeNullNode(node, nullcount++, stream);

		if (node->right)
		{
			fprintf(stream, "    %u -> %u;\n", node->ID, node->right->ID);
			DebugTreeNode(node->right, stream);
		}
		else
			DebugTreeNullNode(node, nullcount++, stream);
	}

	void PhysicsManager::DebugTree(Node* tree, FILE* stream)
	{
		fprintf(stream, "digraph BST {\n");
		fprintf(stream, "    node [fontname=\"Arial\"];\n");

		if (!tree)
			fprintf(stream, "\n");
		else if (!tree->right && !tree->left)
			fprintf(stream, "    %d;\n", tree->ID);
		else
			DebugTreeNode(tree, stream);

		fprintf(stream, "}\n");
	}
	
	void PhysicsManager::Init(rapidjson::Value::Object& data)
	{
		{
#define RIGIDBODY_TYPE(name) mRigidbodyTypesMap[#name] = Body::name;
#include "Hollow/Enums/RigidbodyTypes.enum"
#undef RIGIDBODY_TYPE
		}


		// Parse Collision Mask and set the CollisionMask map
		std::ifstream file(std::string(data["CollisionMask"].GetString()));
		std::string line;
		
		if (file.is_open())
		{
			while (getline(file, line))
			{
				std::istringstream iss(line);
				std::vector<std::string> results(std::istream_iterator<std::string>{iss},
					std::istream_iterator<std::string>());
					
				int id = GameMetaData::Instance().mMapOfGameObjectTypes[results[0]];
				int count = results.size() - 1;
				for(int i = 0; i < count; ++i)
				{
					mCollisionMask[BIT(id) | BIT(i)] = (bool)std::stoi(results[i + 1]);
				}
			}
			file.close();
		}


	}

	void PhysicsManager::UpdateScale(GameObject* pGo)
	{
		Collider* pCol = static_cast<Collider*>(pGo->GetComponent<Collider>());
		if (pCol->mHasCustomScale)
		{
			return;
		}

		if (pCol->mpShape->mType == ShapeType::BOX)
		{
			// update local shape (0.5f because we are updating half extents)
			static_cast<ShapeAABB*>(pCol->mpLocalShape)->mMin = -0.5f * (pCol->mpTr->mScale);
			static_cast<ShapeAABB*>(pCol->mpLocalShape)->mMax = 0.5f * (pCol->mpTr->mScale);

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
		else if (pCol->mpShape->mType == ShapeType::BALL)
		{
			static_cast<ShapeCircle*>(pCol->mpShape)->mCenter = pCol->mpTr->mPosition;
			static_cast<ShapeCircle*>(pCol->mpShape)->mRadius = pCol->mpTr->mScale.x / 2.0f; // this will only be (x||y||z) as its a sphere!
		}
	}
}
