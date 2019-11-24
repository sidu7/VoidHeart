#include <hollowpch.h>
#include "UIButton.h"

#include "Hollow/Managers/UIManager.h"
#include "Hollow/Managers/ResourceManager.h"

namespace Hollow
{
	UIButton UIButton::instance;

	void UIButton::Init()
	{
		mButtonFunction = ButtonFunction::NUM;
		mIsInteractible = true;
		mpPressedImage = nullptr;
		mpReleasedImage = nullptr;
		mpInactiveImage = nullptr;
		mPressedColor = glm::vec3(0.0f);
		mReleasedColor = glm::vec3(0.0f);
		mInactiveColor = glm::vec3(0.0f);

		mButtonFunctionType = "";
		mPressedImagePath = "";
		mReleasedImagePath = "";
		mInactiveImagePath = "";
	}

	void UIButton::Clear()
	{
		mFunctions.clear();
	}

	void UIButton::Serialize(rapidjson::Value::Object data)
	{
		if (data.HasMember("Function"))
		{
			mButtonFunctionType = data["Function"].GetString();
			mButtonFunction = UIManager::Instance().mButtonsEnumMap[mButtonFunctionType];
			mFunctions = UIManager::Instance().mButtonFunctionsMap[mButtonFunction];
		}
		if (data.HasMember("IsInteractible"))
		{
			mIsInteractible = data["IsInteractible"].GetBool();
		}
		if (data.HasMember("PressedImage"))
		{
			mPressedImagePath = data["PressedImage"].GetString();
			mpPressedImage = ResourceManager::Instance().LoadTexture(mPressedImagePath);
		}
		if (data.HasMember("ReleasedImage"))
		{
			mReleasedImagePath = data["PressedImage"].GetString();
			mpReleasedImage = ResourceManager::Instance().LoadTexture(mReleasedImagePath);
		}
		if (data.HasMember("InactiveImage"))
		{
			mInactiveImagePath = data["PressedImage"].GetString();
			mpInactiveImage = ResourceManager::Instance().LoadTexture(mInactiveImagePath);
		}
		if (data.HasMember("PressedColor"))
		{
			mPressedColor = JSONHelper::GetVec3F(data["PressedColor"].GetArray());
		}
		if (data.HasMember("ReleasedColor"))
		{
			mReleasedColor = JSONHelper::GetVec3F(data["ReleasedColor"].GetArray());
		}
		if (data.HasMember("InactiveColor"))
		{
			mInactiveColor = JSONHelper::GetVec3F(data["InactiveColor"].GetArray());
		}
	}

	void UIButton::DeSerialize(rapidjson::Writer<rapidjson::StringBuffer>& writer)
	{
		JSONHelper::Write("Function", mButtonFunctionType, writer);
		JSONHelper::Write("IsInteractible", mIsInteractible, writer);
		JSONHelper::Write("PressedImage", mPressedImagePath, writer);
		JSONHelper::Write("ReleasedImage", mReleasedImagePath, writer);
		JSONHelper::Write("InactiveImage", mInactiveImagePath, writer);
		JSONHelper::Write("PressedColor", mPressedColor, writer);
		JSONHelper::Write("ReleasedColor", mReleasedColor, writer);
		JSONHelper::Write("InactiveColor", mInactiveColor, writer);
	}

	void UIButton::DebugDisplay()
	{
	}
}