#include "hollowpch.h"
#include "Collider.h"
#include "Shape.h"
#include "Transform.h"

namespace Hollow
{

	void Collider::Init()
	{
		mpShape = new ShapeAABB(glm::vec3(0), glm::vec3(0));
		mpShape->mpOwnerCollider = this;

		mpLocalShape = new ShapeAABB(glm::vec3(0), glm::vec3(0));

		coeffRestitution = 0.2f; // bounce
		coeffStaticFriction = 0.9f; // to start sliding
		coeffDynamicFriction = 0.75f; // while sliding
	}

	
	void Collider::Update() {
		UpdateShape();
	}

	void Collider::SetMeshData()
	{
		// generate vertices
		meshData.AddVertex(glm::vec3(-0.5f, -0.5f, 0.5f));
		meshData.AddVertex(glm::vec3(0.5f, -0.5f, 0.5f));
		meshData.AddVertex(glm::vec3(0.5f, 0.5f, 0.5f));
		meshData.AddVertex(glm::vec3(-0.5f, 0.5f, 0.5f));
		meshData.AddVertex(glm::vec3(-0.5f, -0.5f, -0.5f));
		meshData.AddVertex(glm::vec3(0.5f, -0.5f, -0.5f));
		meshData.AddVertex(glm::vec3(0.5f, 0.5f, -0.5f));
		meshData.AddVertex(glm::vec3(-0.5f, 0.5f, -0.5f));

		// generate (quadrilateral) faces
		std::vector<int> faceVerts;
		faceVerts.clear();
		faceVerts.push_back(0);
		faceVerts.push_back(1);
		faceVerts.push_back(2);
		faceVerts.push_back(3);
		meshData.AddFace(faceVerts);

		faceVerts.clear();
		faceVerts.push_back(7);
		faceVerts.push_back(6);
		faceVerts.push_back(5);
		faceVerts.push_back(4);
		meshData.AddFace(faceVerts);
		faceVerts.clear();
		faceVerts.push_back(1);
		faceVerts.push_back(0);
		faceVerts.push_back(4);
		faceVerts.push_back(5);
		meshData.AddFace(faceVerts);
		faceVerts.clear();
		faceVerts.push_back(2);
		faceVerts.push_back(1);
		faceVerts.push_back(5);
		faceVerts.push_back(6);
		meshData.AddFace(faceVerts);
		faceVerts.clear();
		faceVerts.push_back(3);
		faceVerts.push_back(2);
		faceVerts.push_back(6);
		faceVerts.push_back(7);
		meshData.AddFace(faceVerts);
		faceVerts.clear();
		faceVerts.push_back(0);
		faceVerts.push_back(3);
		faceVerts.push_back(7);
		faceVerts.push_back(4);
		meshData.AddFace(faceVerts);
	}

	void Collider::UpdateShape() {
		glm::vec3 extents = static_cast<ShapeAABB*>(mpLocalShape)->GetHalfExtents();
		glm::vec3 x = glm::vec3(extents.x, 0.0f, 0.0f);
		glm::vec3 y = glm::vec3(0.0f, extents.y, 0.0f);
		glm::vec3 z = glm::vec3(0.0f, 0.0f, extents.z);
		glm::vec3 rotatedExtents = abs(glm::mat3(mpTr->mTransformationMatrix) * x) +
			abs(glm::mat3(mpTr->mTransformationMatrix) * y) +
			abs(glm::mat3(mpTr->mTransformationMatrix) * z);

		// based on normalized body vertices
		static_cast<ShapeAABB*>(mpShape)->mMin = glm::vec3(-rotatedExtents.x, -rotatedExtents.y, -rotatedExtents.z) + mpTr->mPosition;
		static_cast<ShapeAABB*>(mpShape)->mMax = glm::vec3(rotatedExtents.x, rotatedExtents.y, rotatedExtents.z) + mpTr->mPosition;
	}
}
