#pragma once
#include "Component.h"

namespace Hollow
{
	class HOLLOW_API UITransform : public Component
	{
		REGISTERCOMPONENT(UITransform);
	public:
		void Init();
		void Clear();
		void Serialize(rapidjson::Value::Object data);
		void DeSerialize(rapidjson::Writer<rapidjson::StringBuffer>& writer);
		void DebugDisplay();

	public:
		glm::vec2 mPosition;
		glm::vec2 mScale;
		float mRotation;
		float mTilt;
		glm::mat4 mTransformationMatrix;
	};
}