#include <hollowpch.h>
#include "Body.h"

namespace Hollow {

	Body Body::instance;

	void Body::Init()
	{
		mMass = 0.0f;
		mInverseMass = 0.0f;
		mPosition = glm::vec3(0);
		mPreviousPosition = glm::vec3(0);
		mVelocity = glm::vec3(0);
		mAngularVelocity = glm::vec3(0);
		mTotalForce = glm::vec3(0);
		mTotalTorque = glm::vec3(0);
		mQuaternion = glm::fquat(0.0f, 0.0f, 0.0f, 1.0f);
		mPreviousQuaternion = glm::fquat(0.0f, 0.0f, 0.0f, 1.0f);
	}

	void Body::Clear()
	{
		Init();
	}

	
	void Body::DebugDisplay()
	{
		if (ImGui::TreeNode("Body"))
		{
			ImGui::InputFloat("Mass", &mMass);
			ImGui::InputFloat3("Position", &mPosition[0]);
			ImGui::InputFloat3("Velocity", &mVelocity[0]);
			ImGui::InputFloat3("AngularVelocity", &mAngularVelocity[0]);
			ImGui::TreePop();
		}
	}

	void Body::Serialize(rapidjson::Value::Object data)
	{
		if (data.HasMember("Mass"))
		{
			mMass = data["Mass"].GetFloat();
		}

		mInverseMass = 1.0f / mMass;
		
		mQuaternion = glm::fquat(0.0f, 0.0f, 0.0f, 1.0f);
	}
	
}