#include <hollowpch.h>
#include "UITransform.h"

namespace Hollow
{
	UITransform UITransform::instance;

	void UITransform::Init()
	{
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