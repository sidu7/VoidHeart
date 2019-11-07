#pragma once
#include "Component.h"

namespace Hollow
{
	class Texture;

	class HOLLOW_API UIButton : public Component
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
		void Init();
		void Clear();
		void Serialize(rapidjson::Value::Object data);
		void DeSerialize(rapidjson::Writer<rapidjson::StringBuffer>& writer);
		void DebugDisplay();

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
