#pragma once
#include "Hollow/Components/UIButton.h"

namespace Hollow
{
#define BUTTON_FUNCTION(x) std::bind(&x,this)

	/// <summary>
	/// Class UI Manager.
	/// </summary>
	class UIManager
	{
		SINGLETON(UIManager);
	public:
		
		/// <summary>
		/// Initializes manager data.
		/// </summary>
		HOLLOW_API  void Init();
		
		/// <summary>
		/// Adds the callback function for [buttonType] button.
		/// </summary>
		/// <param name="buttonType">Type of the button.</param>
		/// <param name="callbackFunction">The callback function.</param>
		HOLLOW_API  void AddButtonFunction(UIButton::ButtonFunction buttonType, std::function<void()> callbackFunction);
		
		/// <summary>
		/// Cleans manager data.
		/// </summary>
		HOLLOW_API  void CleanUp();

		friend class UIButton;
	private:
		std::unordered_map<UIButton::ButtonFunction, std::vector<std::function<void()>>> mButtonFunctionsMap;
		std::unordered_map<std::string, UIButton::ButtonFunction> mButtonsEnumMap;
	};
}