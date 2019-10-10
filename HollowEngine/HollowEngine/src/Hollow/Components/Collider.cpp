#include <hollowpch.h>
#include "Collider.h"
#include "Shape.h"

namespace Hollow
{
	Collider Collider::instance;

	void Collider::Init()
	{
		mpShape = new ShapeAABB(glm::vec3(0), glm::vec3(0));
		mpShape->mpOwnerCollider = this;

		mpLocalShape = new ShapeAABB(glm::vec3(0), glm::vec3(0));

		coeffRestitution = 0.2f; // bounce
		coeffStaticFriction = 0.9f; // to start sliding
		coeffDynamicFriction = 0.75f; // while sliding
	}

	void Collider::Clear()
	{
	}

	void Collider::DebugDisplay()
	{
		if (ImGui::TreeNode("Collider"))
		{
			ImGui::InputFloat3("Shape Extents", &mpShape->GetHalfExtents()[0]);
			ImGui::TreePop();
		}
	}

	void Collider::Serialize(rapidjson::Value::Object data)
	{
		
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

}
