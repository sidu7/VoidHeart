#include <hollowpch.h>

#include "Body.h"
#include "Hollow/Managers/PhysicsManager.h"
#include "Hollow/Utils/ImGuiHelper.h"

namespace Hollow {

	Body Body::instance;

	void Body::Init()
	{
		mMass = 0.0f;
		mInverseMass = 0.0f;
		mPosition = glm::vec3(0);
		mPreviousPosition = glm::vec3(0);
		mVelocity = glm::vec3(0);
		mAngularVelocity = glm::vec3(0.0f, 0.0f, 0.0f);
		mTotalForce = glm::vec3(0);
		mTotalTorque = glm::vec3(0);
		mQuaternion = glm::fquat(0.0f, 0.0f, 0.0f, 1.0f);
		mPreviousQuaternion = glm::fquat(0.0f, 0.0f, 0.0f, 1.0f);
		mIsFrictionLess = false;
		mUseGravity = true;
		
		mDRigidbodyType = "";
		mBodyType = Body::STATIC;
	}

	void Body::Clear()
	{
		Init();
	}

	
	void Body::DebugDisplay()
	{
		ImGui::InputFloat("Mass", &mMass);
		ImGuiHelper::InputText("RigidbodyType", mDRigidbodyType);
		ImGui::InputFloat3("Position", &mPosition[0]);
		ImGui::InputFloat3("Velocity", &mVelocity[0]);
		ImGui::InputFloat3("AngularVelocity", &mAngularVelocity[0]);
		ImGui::Checkbox("Frictionless", &mIsFrictionLess);
		ImGui::Checkbox("Use Gravity", &mUseGravity);
	}

	void Body::Serialize(rapidjson::Value::Object data)
	{
		if (data.HasMember("Mass"))
		{
			mMass = data["Mass"].GetFloat();
		}
		if(data.HasMember("Velocity"))
		{
			mVelocity = JSONHelper::GetVec3F(data["Velocity"].GetArray());
		}
		if (data.HasMember("RigidbodyType"))
		{
			mDRigidbodyType = data["RigidbodyType"].GetString();
			mBodyType = PhysicsManager::Instance().mRigidbodyTypesMap[mDRigidbodyType];

			if(mBodyType == Body::STATIC)
				mMass = std::numeric_limits<float>::infinity();
		}
		if (data.HasMember("IsFrictionLess"))
		{
			mIsFrictionLess = data["IsFrictionLess"].GetBool();
		}
		if (data.HasMember("UseGravity"))
		{
			mUseGravity = data["UseGravity"].GetBool();
		}
		if (data.HasMember("AngularVelocity"))
		{
			mAngularVelocity = JSONHelper::GetVec3F(data["AngularVelocity"].GetArray());
		}

		mInverseMass = 1.0f / mMass;
	}

	HOLLOW_API void Body::DeSerialize(rapidjson::Writer<rapidjson::StringBuffer>& writer)
	{
		if(mMass == std::numeric_limits<float>::infinity())
		{
			JSONHelper::Write<float>("Mass", 0, writer);
		}
		else
		{	
			JSONHelper::Write<float>("Mass", mMass, writer);
		}
		if (mBodyType == STATIC)
		{
			JSONHelper::Write<glm::vec3>("Velocity", mVelocity, writer);
		}
		JSONHelper::Write<std::string>("RigidbodyType", mDRigidbodyType, writer);
		JSONHelper::Write<bool>("IsFrictionLess", mIsFrictionLess, writer);
		JSONHelper::Write<bool>("UseGravity", mUseGravity, writer);
		// TODO: Shantanu can deal with this if it causes problems
		JSONHelper::Write<glm::vec3>("AngularVelocity", mAngularVelocity, writer);
	}
	
}
