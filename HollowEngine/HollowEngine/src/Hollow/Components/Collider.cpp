#include <hollowpch.h>
#include "Collider.h"
#include "Hollow/Physics/Broadphase/Shape.h"

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


	

}
