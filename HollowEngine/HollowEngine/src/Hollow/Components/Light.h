#pragma once
#include "Component.h"

namespace Hollow
{
	class FrameBuffer;

	enum LightType
	{
		GLOBAL,
		LOCAL,
		LIGHT_NUM
	};

	class HOLLOW_API Light : public Component
	{
		REGISTERCOMPONENT(Light);
	public:
		void Init();
		void Serialize(rapidjson::Value::Object data);
		void DeSerialize(rapidjson::Writer<rapidjson::StringBuffer>& writer);
		void Clear();
		void DebugDisplay();

	public:
		glm::vec3 mPosition; // Position of light or offset from the objects center position ?
		float mRadius;
		glm::vec3 mColor;
		bool mCastShadow;
		FrameBuffer* mpShadowMap;
		glm::vec2 mShadowMapSize;
		float mShadowMapNearPlane;
		float mShadowMapFarPlane;
		LightType mType;
		//TODO: type of light
	};
}