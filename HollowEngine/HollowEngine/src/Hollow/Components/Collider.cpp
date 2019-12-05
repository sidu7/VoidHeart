#include <hollowpch.h>
#include "Collider.h"
#include "Hollow/Physics/Broadphase/Shape.h"
#include "Hollow/Utils/ImGuiHelper.h"

namespace Hollow
{
	Collider Collider::instance;

	void Collider::Init()
	{
		mBounciness = 0.2f; // bounce
		mFriction = 0.0f;
		mpLocalShape = nullptr;
		mpShape = new ShapeAABB(glm::vec3(0.0f), glm::vec3(0.0f));
		mpLocalShape = new ShapeAABB(glm::vec3(0.0f), glm::vec3(0.0f));
		mpShape->mpOwnerCollider = this;
	}

	void Collider::Clear()
	{
	}

	void Collider::DebugDisplay()
	{
		ImGui::InputFloat3("Shape Extents", &mpShape->GetHalfExtents()[0]);
		ImGui::Checkbox("Is Trigger", &mIsTrigger);
		ImGui::InputFloat("Bounciness", &mBounciness);
		ImGui::InputFloat("Friction", &mFriction);
		ImGuiHelper::InputText("Shape", mDShape);
	}

	void Collider::Serialize(rapidjson::Value::Object data)
	{
		if (data.HasMember("isTrigger"))
		{
			mIsTrigger = data["isTrigger"].GetBool();
		}
		if (data.HasMember("Bounciness"))
		{
			mBounciness = data["Bounciness"].GetFloat();
		}
		if (data.HasMember("Friction"))
		{
			mFriction = data["Friction"].GetFloat();
		}
		if (data.HasMember("Shape"))
		{
			mDShape = data["Shape"].GetString();
			if (mDShape == "BALL")
			{
				mpShape = new ShapeCircle(0.0f, glm::vec3(0.0f));
				mpLocalShape = new ShapeCircle(0.0f, glm::vec3(0.0f));
				mpShape->mpOwnerCollider = this;
			}
			if (mDShape == "BOX")
			{
				mpShape = new ShapeAABB(glm::vec3(0.0f), glm::vec3(0.0f));
				mpLocalShape = new ShapeAABB(glm::vec3(0.0f), glm::vec3(0.0f));
				mpShape->mpOwnerCollider = this;
			}
		}

	}

	HOLLOW_API void Collider::DeSerialize(rapidjson::Writer<rapidjson::StringBuffer> & writer)
	{
		JSONHelper::Write<float>("Friction", mFriction, writer);
		JSONHelper::Write<float>("Bounciness", mBounciness, writer);
		JSONHelper::Write<std::string>("Shape", mDShape, writer);
		JSONHelper::Write<bool>("isTrigger", mIsTrigger, writer);
	}
}
