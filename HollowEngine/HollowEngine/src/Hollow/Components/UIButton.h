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
		HOLLOW_API void Init();
		HOLLOW_API void Clear();
		HOLLOW_API void Serialize(rapidjson::Value::Object data);
		HOLLOW_API void DeSerialize(rapidjson::Writer<rapidjson::StringBuffer>& writer);
		HOLLOW_API void DebugDisplay();

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
