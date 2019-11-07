#pragma once
#include "Hollow/Components/UIButton.h"

namespace Hollow
{
#define BUTTON_FUNCTION(x) std::bind(&x,this)

	class UIManager
	{
		SINGLETON(UIManager);
	public:
		HOLLOW_API  void Init();
		HOLLOW_API  void AddButtonFunction(UIButton::ButtonFunction buttonType, std::function<void()> callbackFunction);
		HOLLOW_API  void CleanUp();

		friend class UIButton;
	private:
		std::unordered_map<UIButton::ButtonFunction, std::vector<std::function<void()>>> mButtonFunctionsMap;
		std::unordered_map<std::string, UIButton::ButtonFunction> mButtonsEnumMap;
	};
}