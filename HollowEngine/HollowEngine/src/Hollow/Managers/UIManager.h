#pragma once
#include "Hollow/Components/UIButton.h"

namespace Hollow
{
	class HOLLOW_API UIManager
	{
		SINGLETON(UIManager);
	public:
		void Init();
		void AddButtonFunction(UIButton::ButtonFunction buttonType, std::function<void()> callbackFunction);
		void CleanUp();

		friend class UIButton;
	private:
		std::unordered_map<UIButton::ButtonFunction, std::vector<std::function<void()>>> mButtonFunctionsMap;
		std::unordered_map<std::string, UIButton::ButtonFunction> mButtonsEnumMap;
	};
}