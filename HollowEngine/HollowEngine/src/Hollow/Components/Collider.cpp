#include <hollowpch.h>
#include "Collider.h"
#include "Hollow/Physics/Broadphase/Shape.h"

namespace Hollow
{
	Collider Collider::instance;

	void Collider::Init()
	{
		coeffRestitution = 0.2f; // bounce
		coeffStaticFriction = 0.9f; // to start sliding
		coeffDynamicFriction = 0.75f; // while sliding
		mpLocalShape = nullptr;
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
		if (data.HasMember("isTrigger"))
		{
			isTrigger = data["isTrigger"].GetBool();
		}
		if (data.HasMember("kRestitution"))
		{
			coeffRestitution = data["kRestitution"].GetFloat();
		}
		if (data.HasMember("kFriction"))
		{
			coeffDynamicFriction = data["kFriction"].GetFloat();
		}
		if (data.HasMember("Shape"))
		{
			std::string Shape = data["Shape"].GetString();
			if ( Shape == "BALL")
			{
				mpShape = new ShapeCircle(0.0f, glm::vec3(0.0f));
				mpLocalShape = new ShapeCircle(0.0f, glm::vec3(0.0f));
				mpShape->mpOwnerCollider = this;
			}
			if (Shape == "BOX")
			{
				mpShape = new ShapeAABB(glm::vec3(0.0f), glm::vec3(0.0f));
				mpLocalShape = new ShapeAABB(glm::vec3(0.0f), glm::vec3(0.0f));
				mpShape->mpOwnerCollider = this;
			}
		}
	}
}
