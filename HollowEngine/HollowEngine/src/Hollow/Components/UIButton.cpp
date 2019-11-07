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
	}

	void UIButton::Clear()
	{
		mFunctions.clear();
	}

	void UIButton::Serialize(rapidjson::Value::Object data)
	{
		if (data.HasMember("Function"))
		{
			mButtonFunction = UIManager::Instance().mButtonsEnumMap[data["Function"].GetString()];
			mFunctions = UIManager::Instance().mButtonFunctionsMap[mButtonFunction];
		}
		if (data.HasMember("IsInteractible"))
		{
			mIsInteractible = data["IsInteractible"].GetBool();
		}
		if (data.HasMember("PressedImage"))
		{
			mpPressedImage = ResourceManager::Instance().LoadTexture(data["PressedImage"].GetString());
		}
		if (data.HasMember("ReleasedImage"))
		{
			mpReleasedImage = ResourceManager::Instance().LoadTexture(data["PressedImage"].GetString());
		}
		if (data.HasMember("InactiveImage"))
		{
			mpInactiveImage = ResourceManager::Instance().LoadTexture(data["PressedImage"].GetString());
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
	}

	void UIButton::DebugDisplay()
	{
	}
}