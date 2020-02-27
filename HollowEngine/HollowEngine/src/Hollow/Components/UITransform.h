#pragma once
#include "Component.h"

namespace Hollow
{
	class UITransform : public Component
	{
		REGISTERCOMPONENT(UITransform);
	public:
		/// <summary>
		/// Initializes the UITransform component.
		/// </summary>
		HOLLOW_API void Init() override;
		/// <summary>
		/// Clears the UITransform component.
		/// </summary>
		HOLLOW_API void Clear() override;

		/// <summary>
		/// Serializes the specified data of the UITransform component from the JSON file.
		/// </summary>
		/// <param name="data">JSON object.</param>
		HOLLOW_API void Serialize(rapidjson::Value::Object data) override;

		/// <summary>
		/// DeSerialize the current UITransform component into a JSON file.
		/// </summary>
		/// <param name="writer">The writer of the JSON file.</param>
		HOLLOW_API void DeSerialize(rapidjson::Writer<rapidjson::StringBuffer>& writer) override;

		/// <summary>
		/// To set the Debug Display for the UITransform Component.
		/// </summary>
		HOLLOW_API void DebugDisplay() override;

	public:
		glm::vec2 mPosition;
		glm::vec2 mScale;
		float mRotation;
		float mTilt;
		glm::mat4 mTransformationMatrix;
		int mLayer;
	};
}