#pragma once

namespace Hollow
{
	class FrameBuffer;
	enum LightType;

	struct LightData
	{
		glm::vec3 mPosition;
		float mRadius;
		glm::vec3 mColor;
		LightType mType;

		bool mCastShadow;
		FrameBuffer* mpShadowMap;
		glm::mat4 mShadowMatrix;
		float mShadowMapNearPlane;
		float mShadowMapFarPlane;

		unsigned int mBlurDistance;
		float mAlpha;
		float mMD;
	};
}