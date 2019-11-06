#include <hollowpch.h>
#include "UITransform.h"

namespace Hollow
{
	UITransform UITransform::instance;

	void UITransform::Init()
	{
		mPosition = glm::vec2(0.0f);
		mScale = glm::vec2(0.0f);
		mRotation = 0.0f;
		mTilt = 0.0f;
		glm::mat4 mTransformationMatrix = glm::mat4(1.0f);
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
		if (data.HasMember("Position"))
		{
			mScale = JSONHelper::GetVec2F(data["Position"].GetArray());
		}
		if (data.HasMember("Rotation"))
		{
			mRotation = data["Rotation"].GetFloat();
		}
		if (data.HasMember("Tilt"))
		{
			mTilt = data["Tilt"].GetFloat();
		}
	}

	void UITransform::DeSerialize(rapidjson::Writer<rapidjson::StringBuffer>& writer)
	{
	}

	void UITransform::DebugDisplay()
	{
	}
}