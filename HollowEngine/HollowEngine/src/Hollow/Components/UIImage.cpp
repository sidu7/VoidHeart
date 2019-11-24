#include <hollowpch.h>
#include "UIImage.h"

#include "Hollow/Managers/ResourceManager.h"

namespace Hollow
{
	UIImage UIImage::instance;

	void UIImage::Init()
	{
		mpTexture = nullptr;
		mColor = glm::vec3(0.0f);
		mpShapeData = nullptr;
		mShapeType = -1;
	}

	void UIImage::Clear()
	{
	}

	void UIImage::Serialize(rapidjson::Value::Object data)
	{
		if (data.HasMember("Image"))
		{
			TexturePath = data["Image"].GetString();
			mpTexture = ResourceManager::Instance().LoadTexture(TexturePath);
		}
		if (data.HasMember("Shape"))
		{
			mShapeType = data["Shape"].GetUint();
			mpShapeData = ResourceManager::Instance().GetShape((Shapes)mShapeType);
		}
		if (data.HasMember("Color"))
		{
			mColor = JSONHelper::GetVec3F(data["Color"].GetArray());
		}
	}

	void UIImage::DeSerialize(rapidjson::Writer<rapidjson::StringBuffer>& writer)
	{
		JSONHelper::Write("Image", TexturePath, writer);
		JSONHelper::Write("Shape", mShapeType, writer);
		JSONHelper::Write("Color", mColor, writer);
	}

	void UIImage::DebugDisplay()
	{
	}
}