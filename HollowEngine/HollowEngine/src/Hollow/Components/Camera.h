#pragma once
#include "Component.h"

namespace Hollow 
{
	enum CameraType;
	enum CameraProjection;

	class Camera : public Component
	{
		REGISTERCOMPONENT(Camera);
	public:
		HOLLOW_API void Init();
		HOLLOW_API void Serialize(rapidjson::Value::Object data);
		HOLLOW_API void Clear();
		HOLLOW_API void DebugDisplay();
		HOLLOW_API void DeSerialize(rapidjson::Writer<rapidjson::StringBuffer>& writer) override;

	public:
		CameraType mType;
		CameraProjection mProjectionType;
		glm::vec3 mFront;
		glm::vec3 mUp;			//Camera's Up Direction
		glm::vec3 mRight;

		glm::vec3 mOffsetFromAnchor;

		// Top down camera variables
		glm::vec3 mPreviousPosition;
		float mLERPFactor;
		glm::vec2 mXConstraints;
		glm::vec2 mYConstraints;
		glm::vec2 mZConstraints;
		
		bool mIsActive;
		
		float mYaw;
		float mPitch;
		float mAspectRatio;
		
		float mMovementSpeed;
		float mMouseSensitivity;
		float mZoom;
		float mNearPlane;
		float mFarPlane;
		glm::ivec2 mViewPortPosition;
		glm::ivec2 mViewPortSize;

		float mDefaultZoom;		//Used to reset the values
		float mDefaultPitch;	//Used to reset the values
		float mDefaultYaw;		//Used to reset the values

	private:
		glm::ivec2 mDViewPortPosition;
	};
}