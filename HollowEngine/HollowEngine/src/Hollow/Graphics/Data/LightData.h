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
		float mFOV;
		glm::vec3 mLookAtPoint;

		unsigned int mBlurDistance;
		float mAlpha;
		float mMD;
	};
}