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
	}

	void UIImage::Clear()
	{
	}

	void UIImage::Serialize(rapidjson::Value::Object data)
	{
		if (data.HasMember("Image"))
		{
			mpTexture = ResourceManager::Instance().LoadTexture(data["Image"].GetString());
		}
		if (data.HasMember("Shape"))
		{
			mShapeType = (Shapes)data["Shape"].GetUint();
			mpShapeData = ResourceManager::Instance().GetShape(mShapeType);
		}
		if (data.HasMember("Color"))
		{
			mColor = JSONHelper::GetVec3F(data["Color"].GetArray());
		}
	}

	void UIImage::DeSerialize(rapidjson::Writer<rapidjson::StringBuffer>& writer)
	{
	}

	void UIImage::DebugDisplay()
	{
	}
}