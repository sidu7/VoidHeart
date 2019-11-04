#include <hollowpch.h>
#include "Body.h"

namespace Hollow {

	Body Body::instance;

	std::unordered_map<std::string, RigidbodyType> Body::mapOfTypesToStrings;

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
		isFrictionLess = false;

		bodyType = DYNAMIC;

		{
#define RIGIDBODY_TYPE(name) Body::mapOfTypesToStrings[#name] = Hollow::RigidbodyType::name;
#include "RigidbodyTypes.enum"
#undef RIGIDBODY_TYPE
		}
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
		if (data.HasMember("RigidbodyType"))
		{
			bodyType = mapOfTypesToStrings[data["RigidbodyType"].GetString()];

			if(bodyType == STATIC)
				mMass = 1e39;
		}
		if (data.HasMember("isFrictionLess"))
		{
			isFrictionLess = data["isFrictionLess"].GetBool();
		}

		mInverseMass = 1.0f / mMass;
		
		mQuaternion = glm::fquat(0.0f, 0.0f, 0.0f, 1.0f);
	}
	
}