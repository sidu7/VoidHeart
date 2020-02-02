#include <hollowpch.h>
#include "Transform.h"

namespace Hollow {

	Transform Transform::instance;

	void Transform::Init()
	{
		dirtyBit = false;
		mPosition = glm::vec3(0.0f,0.0f,0.0f);					
		mScale = glm::vec3(1.0f, 1.0f, 1.0f);
		mRotation = glm::vec3(0.0f, 0.0f, 0.0f);
		mQuaternion = glm::fquat(0.0f, 0.0f, 0.0f, 1.0f);
		mTransformationMatrix = glm::mat4(1.0f);
		mDebugTransformationMatrix = glm::mat4(1.0f);
		mPreviousTransformationMatrix  = glm::mat4(1.0f);
	}

	void Transform::DebugDisplay()
	{
		ImGui::InputFloat3("Position", &mPosition[0]);
        if(ImGui::InputFloat3("Rotation", &mRotation[0]))
        {
			glm::mat4 rotate = glm::mat4(1.0f);
			rotate = glm::rotate(rotate, glm::radians(mRotation.x),
				glm::vec3(1.0f, 0.0f, 0.0f));
			rotate = glm::rotate(rotate, glm::radians(mRotation.y),
				glm::vec3(0.0f, 1.0f, 0.0f));
			rotate = glm::rotate(rotate, glm::radians(mRotation.z),
				glm::vec3(0.0f, 0.0f, 1.0f));
			mQuaternion = glm::toQuat(rotate);
        }
		ImGui::InputFloat3("Scale", &mScale[0]);
	}

	void Transform::Serialize(rapidjson::Value::Object data)
	{
		if (data.HasMember("Position"))
		{
			mPosition = JSONHelper::GetVec3F(data["Position"].GetArray());
		}
		if (data.HasMember("Scale"))
		{
			mScale = JSONHelper::GetVec3F(data["Scale"].GetArray());
		}
		if (data.HasMember("Rotation"))
		{
			mRotation = JSONHelper::GetVec3F(data["Rotation"].GetArray());
		}
        Rotate(mRotation);
	}

	void Transform::DeSerialize(rapidjson::Writer<rapidjson::StringBuffer>& writer)
	{
		JSONHelper::Write<glm::vec3>("Position", mPosition, writer);
		JSONHelper::Write<glm::vec3>("Rotation", mRotation, writer);
		JSONHelper::Write<glm::vec3>("Scale", mScale, writer);
	}

    glm::vec3 Transform::GetForward() const
    {
        return glm::vec3(mTransformationMatrix[2][0]
                       , mTransformationMatrix[2][1]
                       , mTransformationMatrix[2][2]);
    }

    void Transform::Rotate(const glm::vec3 angles) 
    {
        glm::mat4 rotate = glm::mat4(1.0f);
        rotate = glm::rotate(rotate, glm::radians(angles.x), glm::vec3(1.0f, 0.0f, 0.0f));
        rotate = glm::rotate(rotate, glm::radians(angles.y), glm::vec3(0.0f, 1.0f, 0.0f));
        rotate = glm::rotate(rotate, glm::radians(angles.z), glm::vec3(0.0f, 0.0f, 1.0f));
        mQuaternion = glm::toQuat(rotate);

        glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(model, mPosition);
        model *= rotate;
        model = glm::scale(model, mScale);
        mTransformationMatrix = model;
    }
}