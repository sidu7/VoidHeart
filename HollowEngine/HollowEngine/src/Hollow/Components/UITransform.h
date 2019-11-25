#pragma once
#include "Component.h"

namespace Hollow
{
	class UITransform : public Component
	{
		REGISTERCOMPONENT(UITransform);
	public:
		HOLLOW_API void Init() override;
		HOLLOW_API void Clear() override;
		HOLLOW_API void Serialize(rapidjson::Value::Object data) override;
		HOLLOW_API void DeSerialize(rapidjson::Writer<rapidjson::StringBuffer>& writer) override;
		HOLLOW_API void DebugDisplay() override;

	public:
		glm::vec2 mPosition;
		glm::vec2 mScale;
		float mRotation;
		float mTilt;
		glm::mat4 mTransformationMatrix;
	};
}