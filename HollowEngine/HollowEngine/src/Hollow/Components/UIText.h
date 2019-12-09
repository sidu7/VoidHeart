#pragma once
#include "Component.h"

namespace Hollow
{
	class UIText : public Component
	{
		REGISTERCOMPONENT(UIText)
	public:
		/// <summary>
		/// Destructor of the instance of the <see cref="UIText"/> class.
		/// </summary>
		~UIText() override {}

		/// <summary>
		/// Initializes the UIText component.
		/// </summary>
		void Init() override;

		/// <summary>
		/// Serializes the specified data of the UIText component from the JSON file.
		/// </summary>
		/// <param name="data">JSON object.</param>
		void Serialize(rapidjson::Value::Object data) override;

		/// <summary>
		/// DeSerialize the current UIText component into a JSON file.
		/// </summary>
		/// <param name="writer">The writer of the JSON file.</param>
		void DeSerialize(rapidjson::Writer<rapidjson::StringBuffer>& writer) override;

		/// <summary>
		/// Clears the UIText component.
		/// </summary>
		void Clear() override;

		/// <summary>
		/// To set the Debug Display for the UIText Component.
		/// </summary>
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
