#pragma once
#include "Component.h"

namespace Hollow
{
	class FrameBuffer;

	class HOLLOW_API Light : public Component
	{
		REGISTERCOMPONENT(Light);
	public:
		void Init();
		void Serialize(rapidjson::Value::Object data);
		void Clear();
		void DebugDisplay();

	public:
		glm::vec3 mPosition; // Position of light or offset from the objects center position ?
		float mRadius;
		glm::vec3 mColor;
		bool mCastShadow;
		FrameBuffer* mpShadowMap;
		//TODO: type of light
	};
}