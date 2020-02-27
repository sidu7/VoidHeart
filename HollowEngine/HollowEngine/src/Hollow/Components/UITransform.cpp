#include <hollowpch.h>
#include "UITransform.h"

namespace Hollow
{
	UITransform UITransform::instance;

	void UITransform::Init()
	{
		mPosition = glm::vec2(0.0f);
		mScale = glm::vec2(1.0f);
		mRotation = 0.0f;
		mTilt = 0.0f;
		mTransformationMatrix = glm::mat4(1.0f);
		mLayer = 0;
	}

	void UITransform::Clear()
	{
	}

	void UITransform::Serialize(rapidjson::Value::Object data)
	{
		if (data.HasMember("Position"))
		{
			mPosition = JSONHelper::GetVec2F(data["Position"].GetArray());
		}
		if (data.HasMember("Scale"))
		{
			mScale = JSONHelper::GetVec2F(data["Scale"].GetArray());
		}
		if (data.HasMember("Rotation"))
		{
			mRotation = data["Rotation"].GetFloat();
		}
		if (data.HasMember("Tilt"))
		{
			mTilt = data["Tilt"].GetFloat();
		}
		if (data.HasMember("Layer"))
		{
			mLayer = data["Layer"].GetInt();
		}
	}

	void UITransform::DeSerialize(rapidjson::Writer<rapidjson::StringBuffer>& writer)
	{
		JSONHelper::Write("Position", mPosition, writer);
		JSONHelper::Write("Scale", mScale, writer);
		JSONHelper::Write("Rotation", mRotation, writer);
		JSONHelper::Write("Tilt", mTilt, writer);
		JSONHelper::Write("Layer", mLayer, writer);
	}

	void UITransform::DebugDisplay()
	{
		ImGui::InputFloat2("Position", (float*)&mPosition);
		ImGui::InputFloat2("Scale", (float*)&mScale);
		ImGui::InputFloat("Rotation", &mRotation);
		ImGui::InputFloat("Tilt", &mTilt);
		ImGui::InputInt("Layer", &mLayer);
	}
}