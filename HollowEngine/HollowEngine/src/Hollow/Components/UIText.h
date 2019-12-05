#pragma once
#include "Component.h"

namespace Hollow
{
	class UIText : public Component
	{
		REGISTERCOMPONENT(UIText)
	public:
		~UIText() override {}
		void Init() override;
		void Serialize(rapidjson::Value::Object data) override;
		void DeSerialize(rapidjson::Writer<rapidjson::StringBuffer>& writer) override;
		void Clear() override;
		void DebugDisplay() override;
	public:
		glm::vec2 mOffsetPosition;
		glm::vec2 mTextScale;
		std::string mText;
		std::string mTag;
		glm::vec3 mColor;
		bool mChangingText;
	};
}
