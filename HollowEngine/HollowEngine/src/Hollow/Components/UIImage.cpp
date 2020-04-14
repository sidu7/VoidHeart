#include <hollowpch.h>
#include "UIImage.h"

#include "Hollow/Managers/ResourceManager.h"
#include "Hollow/Utils/ImGuiHelper.h"

namespace Hollow
{
	UIImage UIImage::instance;

	void UIImage::Init()
	{
		mpTexture = nullptr;
		mColor = glm::vec3(0.0f);
		mpShapeData = nullptr;
		mShapeType = "";
        mIsVisible = true;
		mAlpha = 0.0f;
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
			mShapeType = data["Shape"].GetString();
			mpShapeData = ResourceManager::Instance().GetShape(mShapeType);
		}
		if (data.HasMember("Color"))
		{
			mColor = JSONHelper::GetVec3F(data["Color"].GetArray());
		}
		if (data.HasMember("Alpha"))
		{
			mAlpha = data["Alpha"].GetFloat();
		}
	}

	void UIImage::DeSerialize(rapidjson::Writer<rapidjson::StringBuffer>& writer)
	{
		JSONHelper::Write("Image", TexturePath, writer);
		JSONHelper::Write("Shape", mShapeType, writer);
		JSONHelper::Write("Color", mColor, writer);
		JSONHelper::Write("Alpha", mAlpha, writer);
	}

	void UIImage::DebugDisplay()
	{
		ImGuiHelper::InputText("Image File", TexturePath);
		ImGuiHelper::InputText("Shape", mShapeType);
		ImGui::InputFloat3("Color", (float*)&mColor);
		ImGui::InputFloat("Alpha", &mAlpha);
		// TODO: Find better place to update UI image
		mpTexture = ResourceManager::Instance().LoadTexture(TexturePath);
	}

	void UIImage::SetTexture(std::string path)
	{
		TexturePath = path;
		mpTexture = ResourceManager::Instance().LoadTexture(TexturePath);
	}
}
