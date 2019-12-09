#pragma once
#include "Component.h"

namespace Hollow
{
	class Texture;

	class UIButton : public Component
	{
		REGISTERCOMPONENT(UIButton);
	public:
		enum ButtonFunction
		{
#define UIBUTTON(name) name,
#include "Hollow/Enums/Buttons.enum"
#undef UIBUTTON
			NUM
		};

	public:
		/// <summary>
		/// Initializes the UIButton component.
		/// </summary>
		HOLLOW_API void Init() override;

		/// <summary>
		/// Clears the UIButton component.
		/// </summary>
		HOLLOW_API void Clear() override;

		/// <summary>
		/// Serializes the specified data of the UIButton component from the JSON file.
		/// </summary>
		/// <param name="data">JSON object.</param>
		HOLLOW_API void Serialize(rapidjson::Value::Object data) override;

		/// <summary>
		/// DeSerialize the current UIButton component into a JSON file.
		/// </summary>
		/// <param name="writer">The writer of the JSON file.</param>
		HOLLOW_API void DeSerialize(rapidjson::Writer<rapidjson::StringBuffer>& writer) override;

		/// <summary>
		/// To set the Debug Display for the UIButton Component.
		/// </summary>
		HOLLOW_API void DebugDisplay() override;

	private:
		std::string mButtonFunctionType;
		std::string mPressedImagePath;
		std::string mReleasedImagePath;
		std::string mInactiveImagePath;
		
	public:
		std::vector<std::function<void()>> mFunctions;
		ButtonFunction mButtonFunction;
		bool mIsInteractible;
		Texture* mpPressedImage;
		Texture* mpReleasedImage;
		Texture* mpInactiveImage;
		glm::vec3 mPressedColor;
		glm::vec3 mReleasedColor;
		glm::vec3 mInactiveColor;
	};
}
