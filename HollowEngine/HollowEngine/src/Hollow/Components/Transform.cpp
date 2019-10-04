#include <hollowpch.h>
#include "Transform.h"

namespace Hollow {

	Transform Transform::instance;

	void Transform::Init()
	{
		mPosition = glm::vec3(0.0f,0.0f,0.0f);					
		mScale = glm::vec3(1.0f, 1.0f, 1.0f);
		mRotation = glm::vec3(0.0f, 0.0f, 0.0f);
		mTransformationMatrix = glm::mat4(1.0f);
		mDebugTransformationMatrix = glm::mat4(1.0f);
		mPreviousTransformationMatrix  = glm::mat4(1.0f);
	}

	void Transform::DebugDisplay()
	{
		if (ImGui::TreeNode("Transform"))
		{
			ImGui::InputFloat3("Position", &mPosition[0]);
			// TODO: Find a better way to update transform matrix
			glm::mat4 mat(1.0f);
			mat = glm::translate(mat, mPosition);
			SetTranformationMatrix(mat);
			ImGui::TreePop();
		}
	}

	void Transform::Serialize(rapidjson::Value::Object data)
	{
		if (data.HasMember("Position"))
		{
			rapidjson::Value::Array arr = data["Position"].GetArray();
			mPosition = glm::vec3(arr[0].GetFloat(), arr[1].GetFloat(), arr[2].GetFloat());
		}
		if (data.HasMember("Scale"))
		{
			rapidjson::Value::Array arr = data["Scale"].GetArray();
			mScale = glm::vec3(arr[0].GetFloat(), arr[1].GetFloat(), arr[2].GetFloat());
		}
		if (data.HasMember("Rotation"))
		{
			rapidjson::Value::Array arr = data["Rotation"].GetArray();
			mRotation = glm::vec3(arr[0].GetFloat(), arr[1].GetFloat(), arr[2].GetFloat());
		}
		glm::mat4 model = glm::mat4(1.0f);
		model = glm::translate(model, mPosition);
		model = glm::rotate(model, glm::radians(mRotation.x), glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::rotate(model, glm::radians(mRotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::rotate(model, glm::radians(mRotation.z), glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::scale(model, mScale);
		mTransformationMatrix = model;
	}

}