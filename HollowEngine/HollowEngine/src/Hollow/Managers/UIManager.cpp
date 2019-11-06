#include <hollowpch.h>
#include "UIManager.h"

namespace Hollow
{
	void UIManager::Init()
	{
#define UIBUTTON(name) mButtonsEnumMap[#name] = UIButton::ButtonFunction::name;
#include "Hollow/Enums/Buttons.enum"
#undef UIBUTTON
	}

	void UIManager::AddButtonFunction(UIButton::ButtonFunction buttonType, std::function<void()> callbackFunction)
	{
		mButtonFunctionsMap[buttonType].emplace_back(callbackFunction);
	}

	void UIManager::CleanUp()
	{
		mButtonFunctionsMap.clear();
		mButtonsEnumMap.clear();
	}
}