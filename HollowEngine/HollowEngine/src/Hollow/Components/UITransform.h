#pragma once
#include "Component.h"

namespace Hollow
{
	class UITransform : public Component
	{
		REGISTERCOMPONENT(UITransform);
	public:
		HOLLOW_API void Init();
		HOLLOW_API void Clear();
		HOLLOW_API void Serialize(rapidjson::Value::Object data);
		HOLLOW_API void DeSerialize(rapidjson::Writer<rapidjson::StringBuffer>& writer);
		HOLLOW_API void DebugDisplay();

	public:
		glm::vec2 mPosition;
		glm::vec2 mScale;
		float mRotation;
		float mTilt;
		glm::mat4 mTransformationMatrix;
	};
}