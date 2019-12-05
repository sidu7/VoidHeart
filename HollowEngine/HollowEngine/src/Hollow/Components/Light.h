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

	class Light : public Component
	{
		REGISTERCOMPONENT(Light);
	public:
		HOLLOW_API void Init() override;
		HOLLOW_API void Serialize(rapidjson::Value::Object data) override;
		HOLLOW_API void DeSerialize(rapidjson::Writer<rapidjson::StringBuffer>& writer) override;
		HOLLOW_API void Clear() override;
		HOLLOW_API void DebugDisplay() override;

	public:
		glm::vec3 mPosition; // Position of light or offset from the objects center position ?
		float mRadius;
		glm::vec3 mColor;
		glm::vec3 mLookAtPoint;
		LightType mType;

		// Shadow information
		bool mCastShadow;
		FrameBuffer* mpShadowMap;
		glm::vec2 mShadowMapSize;
		float mShadowMapNearPlane;
		float mShadowMapFarPlane;
		float mFOV;

		// Moment shadow map information
		unsigned int mBlurDistance;
		float mAlpha;
		float mMD; // Hopefully can find a good constant or formula to get this
	};
}